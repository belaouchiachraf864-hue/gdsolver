#include <Geode/Geode.hpp>
#include "mod/exe_guard.hpp"

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(GDSolverMenuLayer, MenuLayer) {
    GDSOLVER_HOOK_GUARD()
    bool init() {
        if (!MenuLayer::init()) return false;
        log::info("gdsolver loaded: MenuLayer::init hook fired");
        return true;
    }
};

#include <Geode/modify/PauseLayer.hpp>
class $modify(GDSolverPauseLayer, PauseLayer) {
    GDSOLVER_HOOK_GUARD()

    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto spr = ButtonSprite::create("Solve");
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(GDSolverPauseLayer::onSolve)
        );
        btn->setID("solve-button"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSolve(CCObject*) {
        log::info("gdsolver: Solve button pressed");
        // TODO: appeler ici ton solver
        this->onResume(nullptr); // reprend le niveau
    }
};

$execute {
    log::info("gdsolver: $execute ran (mod binary loaded)");
}
