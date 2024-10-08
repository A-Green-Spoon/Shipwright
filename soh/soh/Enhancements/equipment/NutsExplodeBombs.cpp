#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"
#include "src/overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "src/overlays/actors/ovl_En_Bombf/z_en_bombf.h"
 
static uint32_t nutsExplodeBombsActorInitHookId = 0;
static uint32_t nutsExplodeBombsActorUpdateHookId = 0;
void RegisterNutsExplodeBombs() {
    if (nutsExplodeBombsActorInitHookId) {
        GameInteractor::Instance->UnregisterGameHook<GameInteractor::OnActorInit>(
            nutsExplodeBombsActorInitHookId);
        nutsExplodeBombsActorInitHookId = 0;
    }

    if (nutsExplodeBombsActorUpdateHookId) {
        GameInteractor::Instance->UnregisterGameHook<GameInteractor::OnActorUpdate>(
            nutsExplodeBombsActorUpdateHookId);
        nutsExplodeBombsActorUpdateHookId = 0;
    }

    if (CVarGetInteger(CVAR_ENHANCEMENT("NutsExplodeBombs"), 0)) {
        nutsExplodeBombsActorInitHookId = 
        GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorInit>([](void* refActor) {
            Actor* actor = static_cast<Actor*>(refActor);
            if (actor->id != ACTOR_EN_BOM) {
                return;
            }

            EnBom* enBom = static_cast<EnBom*>(refActor);
            enBom->bombCollider.info.bumper.dmgFlags |= 1;
        });
        
        nutsExplodeBombsActorUpdateHookId = 
        GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorUpdate>([](void* refActor) {
            Actor* actor = static_cast<Actor*>(refActor);
            if (actor->id != ACTOR_EN_BOMBF) {
                return;
            }

            EnBombf* enBombf = static_cast<EnBombf*>(refActor);
            if (actor->params == BOMBFLOWER_BODY && enBombf->timer != 0) {
                enBombf->bombCollider.info.bumper.dmgFlags |= 1;
            }
        });
    }
}