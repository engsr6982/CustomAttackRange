#include "mod/MyMod.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "ll/api/Config.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "ll/api/event/ListenerBase.h"
#include "ll/api/event/player/PlayerClickEvent.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/PlayerActionPacket.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/item/registry/ItemStack.h"


namespace my_mod {

struct Config {
    int                                     version{1};
    std::unordered_map<std::string, double> items; // Example: {"item_name": 1}

    static Config cfg;

    static void load() {
        bool ok = ll::config::loadConfig(Config::cfg, MyMod::getInstance().getSelf().getConfigDir() / "Config.json");
        if (!ok) {
            save();
        }
    }
    static void save() {
        ll::config::saveConfig(Config::cfg, MyMod::getInstance().getSelf().getConfigDir() / "Config.json");
    }
};
Config Config::cfg;


LL_TYPE_INSTANCE_HOOK(
    HandlerPlayerActionPacketHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::handle,
    void,
    NetworkIdentifier const&  source,
    PlayerActionPacket const& packet
) {
    if (auto player = getServerPlayer(source, packet.mClientSubId)) {
        Player&          pl = *player;
        ItemStack const& it = pl.getSelectedItem();

        if (Config::cfg.items.contains(it.getTypeName())) {
            auto const& dt = Config::cfg.items[it.getTypeName()];
            if (dt < 0) {
                return;
            }

            HitResult val = pl.traceRay(static_cast<float>(dt), true, false);
            Actor*    act = val.getEntity();
            if (!act) {
                return;
            }

            pl.attack(*act, ::ActorDamageCause::EntityAttack);
        }
    }
    origin(source, packet);
}


static std::unique_ptr<MyMod> instance;
MyMod&                        MyMod::getInstance() { return *instance; }

bool MyMod::load() {
    getSelf().getLogger().debug("Loading...");
    getSelf().getLogger().info("Author: engsr6982");
    getSelf().getLogger().info("LICENSE: GPLv3");

    Config::load();

    return true;
}

bool MyMod::enable() {
    getSelf().getLogger().debug("Enabling...");

    HandlerPlayerActionPacketHook::hook();

    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().debug("Disabling...");

    HandlerPlayerActionPacketHook::unhook();

    return true;
}

bool MyMod::unload() {
    getSelf().getLogger().debug("Unloading...");
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::instance);
