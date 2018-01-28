this is not a bug, but something I have wondered since I found Premake.

I have the project structure

```
.
├── premake5.lua
├── src
│   ├── example.cpp
│   ├── raw
│   │   ├── attributes.cpp
│   │   ├── ctypes.cpp
│   │   ├── lifetimes.cpp
│   │   ├── locations.cpp
│   │   ├── mutations.cpp
│   │   └── visualisations.cpp
│   ├── test
│   │   └── test_attributes.cpp
│   └── virtaddr.hpp
```

I want to make:
* `src/example.cpp` -> `bin/{config}/example` a `ConsoleApp`
* `src/raw/X.cpp` -> `X.o` a project which can be linked against
* `src/test/test_*.cpp` -> `bin/{config}/test_vaddr` one executable made from all test files
* `bin/{config}/libvirt_addr.a`, a `staticlib` `.a` file

To this end, my `premake5.lua` says

```lua
workspace "virtual-addressing"

  cppdialect "C++11"

  configurations { "dbg", "dist" }

  targetdir "bin/%{cfg.buildcfg}/"

  project "example"
    kind "consoleapp"

    files { path.join("src", "example.cpp") }
    links {
      "virt-addr", "attrs", "ctypes", "lifes", "locats", "muts", "vis",
    }

  project "virt-addr"
    kind "staticlib"

    links { "attrs", "ctypes", "lifes", "locats", "muts", "vis" }

  project "attrs"
    kind "staticlib"
    files { path.join("src", "raw", "attributes.cpp") }

  project "ctypes"
    kind "staticlib"
    files { path.join("src", "raw", "ctypes.cpp") }

  project "lifes"
    kind "staticlib"
    files { path.join("src", "raw", "lifetimes.cpp") }

  project "locats"
    kind "staticlib"
    files { path.join("src", "raw", "locations.cpp") }

  project "muts"
    kind "staticlib"
    files { path.join("src", "raw", "mutations.cpp") }

  project "vis"
    kind "staticlib"
    files { path.join("src", "raw", "visualisations.cpp") }

  project "test"
    kind "consoleapp"

    files { path.join("src", "test", "*.cpp") }
    links {
      "criterion", "virt-addr", "attrs", "ctypes", "lifes", "locats", "muts", "vis",
    }

    targetname "test_vaddr"
```

this is okay, unfortunately though if I want to change something in each of the small projects like `attrs` `ctypes` etc, I must manually change many lines.

it would be good if I could list in bulk some file pattern and have Premake generate projects for each file matching the pattern. GNU Makefiles already have this in a concise way so that it is easy to do what I want, but I want to use Premake because GNU Make is a headache.

to this end, I have written the following `premake5.lua` with the same effect as the above.

```lua
function table.reverse(a)
    local res = {}
    for i = #a, 1, -1 do
        res[#res+1] = a[i]
    end
    return res
end

workspace "virtual-addressing"

  cppdialect "C++11"

  configurations { "dbg", "dist" }

  targetdir "bin/%{cfg.buildcfg}/"

  local proj_names = {}

  for _, file in ipairs(os.matchfiles("src/raw/*.cpp")) do
    local basename = string.explode(file, "/")[3]
    local ident = string.explode(basename, "%.")[1]
    table.insert(proj_names, ident)
    project "*"

    project ( ident )
      kind ( "staticlib" )
      files ( { file } )

    project "*"
  end

  local main_project = "virt_addr"

  local base_links = table.merge(proj_names, { [20] = main_project })

  for k, v in next, base_links do
    print(k, v)
  end

  project "example"
    kind "consoleapp"

    files { path.join("src", "example.cpp") }
    links ( base_links )

  project(main_project)
    kind "staticlib"
    -- don't link "virt-addr" to itself
    links ( proj_names )

  project "test"
    kind "consoleapp"

    files { path.join("src", "test", "test_*.cpp") }

    local test_links = table.merge({ [19] = "criterion" }, base_links)
    test_links = table.reverse(test_links)
    for k, v in next, test_links do
      print(k, v)
    end

    links ( test_links )

    targetname "test_vaddr"
```

`table.reverse` is not in Premake (I will create a pull request for this useful thing), but I am only using it to hack around a problem with Premake's `table.merge`. we will come upon this in another GitHub issue.

the first loop finds all the `.cpp` files in `src/raw`. it generates a project by the name of the each file and gives it the `files` of its own singular source file.

note that the explicit index in the second argument of `table.merge(tbl, { [N] = value })` is a hack to get around the useless behaviour of Premake's `table.merge` to overwrite array elements with the same index. (Here `N` must be strictly larger than any index already in `tbl`).

the contents of `base_links` and their indicies are printed for exposition's sake.

the project `example` links `base_links`, which now consists of all the projects made from `src/raw/*.cpp` and `virt_addr`.

the project `virt_addr` (`project(main_project)`) does not link against itself, but links the rest of the `staticlib` projects thus far (in `proj_names`).

the project `test` consists of all the `.cpp` files in `src/test/`.

again, to add the testing library, we need the `table.merge(tbl, { [N] = value })` hack to prevent `criterion` overwriting or being overwritten in merge.

by its nature `libcriterion` must be listed before all or most other libraries, but it is almost impossible to simply and concisely insert to the beginning of a table. I could have listed `criterion` when initialising `proj_names` but then every other project would be linked against `libcriterion` which is not a very good thing.

`criterion` was just merged into `base_links`, so it is at the end. therefore an easy solution is to reverse the table of project names.

`test_links` is printed so that it is easy to see the effect of the `table.merge` hack.

`test` is linked against `test_links`, and we're done.

Instead of that composition of hacks, I would like to write:

```lua
  --- snip ---
  ProjectGroup "small_objects"
    kind "staticlib"
    -- a function which denotes how to create individual projects from the ProjectGroup
    ProjectGroupMembers "eachFile" -- create a (sub) project for every file given by files()
    files { "src/raw/*.cpp" }      -- files() like this, or a more generic solution
    links { "m" }                  -- trivial example since we are not done linking yet

  --- later ---
  project "example"
    kind "consoleapp"
    files { "src/example.cpp"}
    links { "criterion", "small_objects" }  -- this links example against every staticlib from small_objects
```

what do you think? there are many codebases I would convert from GNU Makefile to Premake, except that Premake is missing this bulk-object / bulk-project generation.
