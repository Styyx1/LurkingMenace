-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse-ng")

-- set project
set_project("SurpriseSpawner")
set_version("3.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- packages
add_requires("simpleini", "nlohmann_json")
add_requires("spdlog", { configs = { header_only = false } })

-- targets
target("SurpriseSpawner")
    -- add dependencies to target
    add_deps("commonlibsse-ng")
    add_packages("fmt", "spdlog", "simpleini", "nlohmann_json")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "SurpriseSpawner",
        author = "Styyx",
        description = "SKSE64 plugin template using CommonLibSSE-NG"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

add_extrafiles("release/**.ini")
add_extrafiles("release/**.json")

after_build(function(target)
    local copy = function(env, ext)
        for _, env in pairs(env:split(";")) do
            if os.exists(env) then
                local plugins = path.join(env, ext, "SKSE/Plugins")
                os.mkdir(plugins)
                os.trycp(target:targetfile(), plugins)
                os.trycp(target:symbolfile(), plugins)
                os.trycp("$(projectdir)/release/**.ini", plugins)
                os.trycp("$(projectdir)/release/**.json", plugins)
            end
        end
    end
    if os.getenv("XSE_TES5_MODS_PATH") then
        copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
    elseif os.getenv("XSE_TES5_GAME_PATH") then
        copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
    end
end)