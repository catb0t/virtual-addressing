workspace "virtual-addressing"

  language "C"
  cdialect "C11"

  configurations { "dbg", "dist" }

  flags { "fatalwarnings" }

  targetdir "bin/%{cfg.buildcfg}/"

  filter { "action:gmake*", "toolset:gcc" }
    buildoptions {
      "-Wall", "-std=c11", "-Wextra", "-Wfloat-equal", "-Winline", "-Wundef", "-Werror",
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

    files { path.join("src", "example.c") }
    links { "virt-addr" }

  project "virt-addr"
    kind "staticlib"

    files { path.join("src", "unify_vaddr.c") }

  project "test"
    kind "consoleapp"

    files { path.join("src", "test", "*.c") }
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
