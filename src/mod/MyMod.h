#pragma once

#include "ll/api/plugin/NativePlugin.h"

namespace my_mod {

class MyMod {

public:
    static MyMod& getInstance();

    MyMod(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    bool unload();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace my_mod
