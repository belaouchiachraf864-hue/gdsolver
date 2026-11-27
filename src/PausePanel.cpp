#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "mod/exe_guard.hpp"
#include "mod/playlayer_helpers.hpp"

using namespace geode::prelude;
using namespace p1;

class $modify(GDSolverPauseLayer, PauseLayer) {
    GDSOLVER_HOOK_GUARD()

    void customSetup() {
        PauseLayer::customSetup();

        log::info("gdsolver: PauseLayer::customSetup hook fired");

        auto win = CCDirector::get()->getWinSize();

        auto menu = CCMenu::create();
        menu->setID("gdsolver-menu"_spr);
        menu->setPosition({0.f, 0.f});
        menu->setContentSize(win);

        float y = win.height / 2.f + 20.f;
        for (auto [label, mode] : {
            std::pair{"Solve", (int)UI_MODE_SOLVE},
            std::pair{"Replay", (int)UI_MODE_REPLAY},
        }) {
            auto spr = ButtonSprite::create(label, "goldFont.fnt", "GJ_button_01.png", .7f);
            auto btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(GDSolverPauseLayer::onBotMode)
            );
            btn->setTag(mode);
            btn->setID(fmt::format("{}-button"_spr, label));
            btn->setPosition({win.width - 60.f, y});
            menu->addChild(btn);
            y -= 40.f;
        }

        this->addChild(menu, 100);
    }

    void onBotMode(CCObject* sender) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;
        auto level = pl->m_level;

        if (level->isPlatformer()) {
            Notification::create("Platformer: not supported", NotificationIcon::Error)->show();
            return;
        }

        int mode = static_cast<CCNode*>(sender)->getTag();
        auto mod = Mod::get();
        bool coins = mod->getSavedValue<bool>("play-coins", false);

        if (mode == UI_MODE_REPLAY) {
            std::vector<InputCmd> plan;
            if (!loadInputsFile(uiSolutionPath(level->m_levelID.value(), coins), plan, true)) {
                Notification::create("No stored solution yet. Solve it first.", NotificationIcon::Warning)->show();
                return;
            }
        }

        g_uiMode = mode;
        g_uiCoins = coins;
        int glitch = mode == UI_MODE_SOLVE
            ? mod->getSavedValue<int>("play-glitch-mode", UI_GLITCH_OFF) : UI_GLITCH_OFF;
        g_uiGlitchMode = glitch;
        g_uiGlitch = glitch != UI_GLITCH_OFF;
        g_uiSecSolve = mode != UI_MODE_SOLVE || mod->getSavedValue<bool>("play-secsolve", true);
        g_uiReplayStep = mode == UI_MODE_REPLAY && mod->getSavedValue<bool>("play-replaystep", false);
        g_uiSearch = mode == UI_MODE_SOLVE
            ? mod->getSavedValue<int>("play-search", UI_SEARCH_DP) : UI_SEARCH_DP;
        g_uiArmedLevel = level;

        auto scene = PlayLayer::scene(level, false);
        CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }
};
