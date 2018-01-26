workspace "virtual-addressing"

  language "C++"

  cppdialect "C++11"

  configurations { "dbg", "dist" }

  flags { "fatalwarnings" }

  targetdir "bin/%{cfg.buildcfg}/"

  buildoptions { "-Wall", "-std=c++11", "-Wextra", "-Wfloat-equal", "-Winline", "-Wundef", "-Werror" }

  filter { "action:gmake*", "toolset:gcc", "language:c" }
    buildoptions {
      "-Wall", "-std=c++11", "-Wextra", "-Wfloat-equal", "-Winline", "-Wundef", "-Werror",
      "-fverbose-asm", "-Wint-to-pointer-cast", "-Wshadow", "-Wpointer-arith",
      "-Wcast-align", "-Wcast-qual", "-Wunreachable-code", "-Wstrict-overflow=5",
      "-Wwrite-strings", "-Wconversion", "--pedantic-errors",
      "-Wredundant-decls", "-Werror=maybe-uninitialized",
      "-Wmissing-declarations", "-Wmissing-parameter-type",
      "-Wmissing-prototypes", "-Wnested-externs", "-Wold-style-declaration",
      "-Wold-style-definition", "-Wstrict-prototypes", "-Wpointer-sign"
    }
    flags { "linktimeoptimization" }

  filter "configurations:dbg"
    buildoptions { "-ggdb", "-O0", "-g3" }
    symbols "on"
    optimize "off"

  filter "configurations:dist"
    buildoptions { "-fomit-frame-pointer", "-O3" }
    symbols "off"
    optimize "full"

  project "example"
    kind "consoleapp"

    files { path.join("src", "example.cpp") }
    links { "virt-addr" }

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
    links { "criterion", "virt-addr" }

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
