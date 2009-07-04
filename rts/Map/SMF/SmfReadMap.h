#ifndef SMFREADMAP_H
#define SMFREADMAP_H

#include "SmfMapFile.h"

class CBFGroundDrawer;
class CCamera;

class CSmfReadMap : public CReadMap
{
public:
	CR_DECLARE(CSmfReadMap)

	CSmfReadMap(std::string mapname);
	~CSmfReadMap();

	void HeightmapUpdatedNow(int x1, int x2, int y1, int y2);
#if !defined HEADLESS
	GLuint GetShadingTexture () { return shadowTex; }
	GLuint GetGrassShadingTexture () { return minimapTex; }
#endif // !defined HEADLESS
	void DrawMinimap ();
	void GridVisibility(CCamera *cam, int quadSize, float maxdist, IQuadDrawer *cb, int extraSize);
	CBaseGroundDrawer* GetGroundDrawer();
	const float* GetHeightmap() { return heightmap; }

	inline void SetHeight(int idx, float h) {
		heightmap[idx] = h;
		currMinHeight = std::min(h, currMinHeight);
		currMaxHeight = std::max(h, currMaxHeight);
	}
	inline void AddHeight(int idx, float a) {
		heightmap[idx] += a;
		currMinHeight = std::min(heightmap[idx], currMinHeight);
		currMaxHeight = std::max(heightmap[idx], currMaxHeight);
	}

	int GetNumFeatureTypes ();
	int GetNumFeatures ();
	void GetFeatureInfo (MapFeatureInfo* f);// returns all feature info in MapFeatureInfo[NumFeatures]
	const char *GetFeatureType (int typeID);

	unsigned char *GetInfoMap (const std::string& name, MapBitmapInfo* bm);
	void FreeInfoMap(const std::string& name, unsigned char *data);

	// todo: do not use, just here for backward compatibility with BFGroundTextures.cpp
	CSmfMapFile& GetFile() { return file; }

	bool usePBO;
	float anisotropy;

protected:
	void ConfigureAnisotropy();

	CSmfMapFile file;

#if !defined HEADLESS
	std::string detailTexName;
	GLuint detailTex;
#endif // !defined HEADLESS

	unsigned char waterHeightColors[1024*4];

#if !defined HEADLESS
	GLuint shadowTex;
	GLuint minimapTex;
#endif // !defined HEADLESS

	float* heightmap;

#if !defined HEADLESS
	CBFGroundDrawer *groundDrawer;
#endif // !defined HEADLESS

	float3 GetLightValue(int x, int y);
	void ParseSMD(std::string filename);

	friend class CBFGroundDrawer;
};

#endif
