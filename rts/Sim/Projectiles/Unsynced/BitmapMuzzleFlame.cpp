#include "StdAfx.h"
#include "mmgr.h"

#include "BitmapMuzzleFlame.h"
#include "Sim/Misc/GlobalSynced.h"
#if !defined HEADLESS
#include "Rendering/GL/VertexArray.h"
#include "Rendering/Textures/ColorMap.h"
#include "Rendering/Textures/TextureAtlas.h"
#endif // !defined HEADLESS
#include "Sim/Projectiles/ProjectileHandler.h"
#include "GlobalUnsynced.h"

CR_BIND_DERIVED(CBitmapMuzzleFlame, CProjectile, );

CR_REG_METADATA(CBitmapMuzzleFlame,
(
	CR_MEMBER_BEGINFLAG(CM_Config),
#if !defined HEADLESS
		CR_MEMBER(sideTexture),
		CR_MEMBER(frontTexture),
#endif // !defined HEADLESS
		CR_MEMBER(dir),
#if !defined HEADLESS
		CR_MEMBER(colorMap),
#endif // !defined HEADLESS
		CR_MEMBER(size),
		CR_MEMBER(length),
		CR_MEMBER(sizeGrowth),
		CR_MEMBER(ttl),
		CR_MEMBER(frontOffset),
	CR_MEMBER_ENDFLAG(CM_Config),
	CR_RESERVED(8)
));

CBitmapMuzzleFlame::CBitmapMuzzleFlame(void) :
	CProjectile()
{
	deleteMe=false;
	checkCol=false;
	useAirLos=true;
	synced = false;
}

CBitmapMuzzleFlame::~CBitmapMuzzleFlame(void)
{
}

void CBitmapMuzzleFlame::Draw(void)
{
#if !defined HEADLESS
	inArray = true;
	life = (gs->frameNum - createTime + gu->timeOffset) * invttl;

	unsigned char col[4];
	colorMap->GetColor(col, life);

	float invlife = 1 - life;
	float igrowth = sizeGrowth * (1 - invlife * invlife);
	float isize = size + size * igrowth;
	float ilength = length + length * igrowth;

	float3 sidedir = dir.cross(float3(0, 1, 0)).ANormalize();
	float3 updir = dir.cross(sidedir).ANormalize();

	va->AddVertexTC(pos + updir * isize,                 sideTexture->xstart, sideTexture->ystart, col);
	va->AddVertexTC(pos + updir * isize + dir * ilength, sideTexture->xend,   sideTexture->ystart, col);
	va->AddVertexTC(pos - updir * isize + dir * ilength, sideTexture->xend,   sideTexture->yend,   col);
	va->AddVertexTC(pos - updir * isize,                 sideTexture->xstart, sideTexture->yend,   col);

	va->AddVertexTC(pos + sidedir * isize,                 sideTexture->xstart, sideTexture->ystart, col);
	va->AddVertexTC(pos + sidedir * isize + dir * ilength, sideTexture->xend,   sideTexture->ystart, col);
	va->AddVertexTC(pos - sidedir * isize + dir * ilength, sideTexture->xend,   sideTexture->yend,   col);
	va->AddVertexTC(pos - sidedir * isize,                 sideTexture->xstart, sideTexture->yend,   col);

	float3 frontpos = pos + dir * frontOffset * ilength;

	va->AddVertexTC(frontpos - sidedir * isize + updir * isize, frontTexture->xstart, frontTexture->ystart, col);
	va->AddVertexTC(frontpos + sidedir * isize + updir * isize, frontTexture->xend,   frontTexture->ystart, col);
	va->AddVertexTC(frontpos + sidedir * isize - updir * isize, frontTexture->xend,   frontTexture->yend,   col);
	va->AddVertexTC(frontpos - sidedir * isize - updir * isize, frontTexture->xstart, frontTexture->yend,   col);
#endif // !defined HEADLESS
}

void CBitmapMuzzleFlame::Update(void)
{
	ttl--;
	if (!ttl) {
		deleteMe = true;
	}
}

void CBitmapMuzzleFlame::Init(const float3 &pos, CUnit *owner GML_PARG_C)
{
	CProjectile::Init(pos, owner GML_PARG_P);
	life = 0;
	createTime = gs->frameNum;
	invttl = 1.0f/(float)ttl;
}
