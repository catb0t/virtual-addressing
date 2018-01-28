function table.reverse(a)
    local res = {}
    for i = #a, 1, -1 do
        res[#res+1] = a[i]
    end
    return res
end

workspace "virtual-addressing"

  language "C++"

  cppdialect "C++11"

  configurations { "dbg", "dist" }

  flags { "fatalwarnings" }

  buildoptions { "-Wl,--no-as-needed" }

  targetdir "bin/%{cfg.buildcfg}/"

  filter { "action:gmake*" }
    buildoptions {
      "-Wall", "-std=c++11", "-Wextra", "-Wfloat-equal", "-Winline", "-Wundef", "-Werror",
      "-fverbose-asm", "-Wint-to-pointer-cast", "-Wshadow", "-Wpointer-arith",
      "-Wcast-align", "-Wcast-qual", "-Wunreachable-code", "-Wstrict-overflow=5",
      "-Wwrite-strings", "-Wconversion", "--pedantic-errors",
      "-Wredundant-decls", "-Wmissing-declarations", "-Werror=uninitialized"
    }

  filter { "toolset:gcc" }
    buildoptions {
      "-Werror=maybe-uninitialized"
    }
    flags { "linktimeoptimization" }


  filter "configurations:dbg"
    buildoptions {
      "-ggdb", "-O0", "-ggdb3", "-fno-omit-frame-pointer"
      -- "-fsanitize=address", "-fstack-protector", "-fsanitize=undefined"
    }
    symbols "on"
    optimize "off"

  filter "configurations:dist"
    buildoptions { "-fomit-frame-pointer", "-O3" }
    symbols "off"
    optimize "full"

  filter {}

  local proj_names = {}

  for _, file in ipairs(os.matchfiles("src/raw/*.cpp")) do
    local basename = string.explode(file, "/")[3]
    local ident = string.explode(basename, "%.")[1]
    -- print("ident", ident)
    table.insert(proj_names, ident)
    project "*"

    project ( ident )
      kind ( "staticlib" )
      files ( { file } )

    project "*"
  end

  local main_project = "virt_addr"

  local base_links = table.merge(proj_names, { [20] = main_project })

  -- for k, v in next, base_links do print(k, v) end

  project "example"
    kind "consoleapp"

    files { path.join("src", "example.cpp") }
    links ( base_links )

  project(main_project)
    kind "staticlib"
    -- don't link main_project to itself
    links ( proj_names )

  project "test"
    kind "consoleapp"

    files { path.join("src", "test", "test_*.cpp") }

    local test_links = table.merge({ [7] = "criterion" }, base_links)
    test_links = table.reverse(test_links)
    -- for k, v in next, test_links do print(k, v) end

    links ( test_links )

    targetname "test_vaddr"

  project "clobber"
    kind "makefile"

    -- not windows, clean like this
    filter "system:not windows"
      cleancommands {
        "({RMDIR} bin obj *.make Makefile *.o -r 2>/dev/null; echo)"
      }

    -- otherwise, clean like this
    filter "system:windows"
      cleancommands {
        "{DELETE} *.make Makefile *.o",
        "{RMDIR} bin obj"
      }
