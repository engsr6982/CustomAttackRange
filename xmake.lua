add_rules("mode.debug", "mode.release")

add_repositories("engsr6982-repo https://github.com/engsr6982/xmake-repo.git")
add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("levilamina_build_cache 0.12.4")

-- add_requires(
--     "fmt 10.2.1",
--     "expected-lite v0.7.0",
--     "gsl v4.0.0",
--     "leveldb 1.23",
--     "magic_enum v0.9.5",
--     "nlohmann_json v3.11.3",
--     "rapidjson v1.1.0",
--     "ctre 3.8.1",
--     "pcg_cpp v1.0.0",
--     "pfr 2.1.1",
--     "symbolprovider v1.1.0",
--     "bdslibrary 1.20.81.01",
--     "entt 3.13.2"
-- )


if not has_config("vs_runtime") then
    set_runtimes("MD")
end

target("CustomAttackRange") -- Change this to your mod name.
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4",
        "/w44265",
        "/w44289",
        "/w44296",
        "/w45263",
        "/w44738",
        "/w45204"
    )
    add_defines("NOMINMAX", "UNICODE")
    add_files("src/**.cpp")
    add_includedirs("src")

    add_packages("levilamina_build_cache")

    -- add_packages(
    --     "fmt",
    --     "expected-lite",
    --     "gsl",
    --     "leveldb",
    --     "magic_enum",
    --     "nlohmann_json",
    --     "rapidjson",
    --     "ctre",
    --     "pcg_cpp",
    --     "pfr",
    --     "symbolprovider",
    --     "bdslibrary",
    --     "entt"
    -- )
    -- add_includedirs("C:/Users/X/AppData/Local/.xmake/packages/l/levilamina/0.12.4/2c31df540f804e3d9a9f06338254cc4d/include")
    -- add_links("C:/Users/X/AppData/Local/.xmake/packages/l/levilamina/0.12.4/2c31df540f804e3d9a9f06338254cc4d/lib/LeviLamina.lib")

    add_shflags("/DELAYLOAD:bedrock_server.dll") -- To use symbols provided by SymbolProvider.
    set_exceptions("none") -- To avoid conflicts with /EHa.
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")

    after_build(function (target)
        local mod_packer = import("scripts.after_build")

        local tag = os.iorun("git describe --tags --abbrev=0 --always")
        local major, minor, patch, suffix = tag:match("v(%d+)%.(%d+)%.(%d+)(.*)")
        if not major then
            print("Failed to parse version tag, using 0.0.0")
            major, minor, patch = 0, 0, 0
        end
        local mod_define = {
            modName = target:name(),
            modFile = path.filename(target:targetfile()),
            modVersion = major .. "." .. minor .. "." .. patch,
        }
        
        mod_packer.pack_mod(target,mod_define)
    end)
