#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "mod/exe_guard.hpp"

using namespace geode::prelude;

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
        log::info("gdsolver: Solve pressed from pause menu");
        // TODO: lancer le solver ici (même appel que le choix « Solve » du menu Play)
    }
};
