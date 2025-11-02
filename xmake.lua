-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("extern/styyx-utils/xmake.lua")

-- set project
set_project("lurking-menace")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("build.optimization.lto", true)
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set configs
set_config("skyrim_ae", true)
set_config("commonlib_toml", true)

-- packages
add_requires("nlohmann_json")

-- targets
target("lurking-menace")
    -- add dependencies to target
    add_deps("commonlibsse")
    add_packages("nlohmann_json")
    add_deps("styyx-util")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "lurking-menace",
        author = "Styyx",
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
    add_extrafiles("release/**.json", {public=true}, "release/**.toml", {public=true})

after_build(function(target)
    local copy = function(env, ext)
        for _, env in pairs(env:split(";")) do
            if os.exists(env) then
                local plugins = path.join(env, ext, "SKSE/Plugins")
                os.mkdir(plugins)
                os.trycp(target:targetfile(), plugins)
                os.trycp(target:symbolfile(), plugins)
                os.trycp("$(projectdir)/release/**.json", plugins)
                os.trycp("$(projectdir)/release/**.toml", plugins)
            end
        end
    end
    if os.getenv("XSE_TES5_MODS_PATH") then
        copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
    elseif os.getenv("XSE_TES5_GAME_PATH") then
        copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
    end
end)