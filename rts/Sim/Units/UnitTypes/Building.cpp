#include "StdAfx.h"
// Building.cpp: implementation of the CBuilding class.
//
//////////////////////////////////////////////////////////////////////

#include "Building.h"
#include "Map/ReadMap.h"
#include "Sim/Units/UnitDef.h"
#include "Game/GameSetup.h"
#if !defined HEADLESS
#include "Rendering/GroundDecalHandler.h"
#include "Rendering/UnitModels/UnitDrawer.h"
#include "Rendering/UnitModels/3DModel.h"
#endif // !defined HEADLESS
#include "GlobalUnsynced.h"

#include "mmgr.h"

CR_BIND_DERIVED(CBuilding, CUnit, );

CR_REG_METADATA(CBuilding, (
				CR_RESERVED(8),
				CR_POSTLOAD(PostLoad)
				));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuilding::CBuilding()
: buildingDecal(0)
{
	immobile=true;
}

// FIXME -- adjust decals for decoys? gets weird?
CBuilding::~CBuilding()
{
#if !defined HEADLESS
	CUnitDrawer::GhostBuilding* gb = NULL;

	if (unitDrawer && (!gameSetup || gameSetup->ghostedBuildings)) {
		if (!(losStatus[gu->myAllyTeam] & (LOS_INLOS | LOS_CONTRADAR)) &&
		     (losStatus[gu->myAllyTeam] & (LOS_PREVLOS)) &&
		    !gu->spectatingFullView) {

			const UnitDef* decoyDef = unitDef->decoyDef;
			S3DModel* gbModel =
				(decoyDef == NULL) ? model : decoyDef->LoadModel();

			gb = new CUnitDrawer::GhostBuilding;
			gb->pos    = pos;
			gb->model  = gbModel;
			gb->decal  = buildingDecal;
			gb->facing = buildFacing;
			gb->team   = team;
			if (gbModel->type == MODELTYPE_S3O) {
				unitDrawer->ghostBuildingsS3O.push_back(gb); // S3O
			} else {
				unitDrawer->ghostBuildings.push_back(gb);    // 3DO
			}
		}
	}

	if (groundDecals && buildingDecal) {
		groundDecals->RemoveBuilding(this, gb);
	}
#endif // !defined HEADLESS
}


void CBuilding::Init(const CUnit* builder)
{
	mass = 100000.0f;
	physicalState = OnGround;

#if !defined HEADLESS
	if (unitDef->useBuildingGroundDecal) {
		groundDecals->AddBuilding(this);
	}
#endif // !defined HEADLESS
	CUnit::Init(builder);
}


void CBuilding::PostLoad()
{
#if !defined HEADLESS
	if (unitDef->useBuildingGroundDecal) {
		groundDecals->AddBuilding(this);
	}
#endif // !defined HEADLESS
}


void CBuilding::UnitInit(const UnitDef* def, int team, const float3& position)
{
	if (def->levelGround) {
		blockHeightChanges = true;
	}
	CUnit::UnitInit(def, team, position);
}
