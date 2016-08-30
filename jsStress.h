#pragma once
#include "common.h"

class JSStress : public MPxDeformerNode
{
public:
	JSStress();
	virtual ~JSStress();
	static void* creator();
	static MStatus initialize();

	virtual MStatus deform(MDataBlock &data, MItGeometry &iterGeo, const MMatrix &toWorldMatrix, unsigned int indexGeo);

	static const char* kName;
	static MTypeId kId;
};