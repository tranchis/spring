#ifndef UNIT_DEF_IMAGE
#define UNIT_DEF_IMAGE

#include "creg/creg_cond.h"
#if !defined HEADLESS
#include "Rendering/GL/myGL.h"
#endif // !defined HEADLESS

struct UnitDefImage
{
	CR_DECLARE_STRUCT(UnitDefImage);
	UnitDefImage() { imageSizeX = -1; imageSizeY = -1; }

	int imageSizeX;
	int imageSizeY;
#if !defined HEADLESS
	GLuint textureID;
#endif // !defined HEADLESS
	bool textureOwner;
};

#endif // UNIT_DEF_IMAGE
