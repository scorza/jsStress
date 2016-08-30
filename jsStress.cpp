#include "common.h"
#include "jsStress.h"

const char* JSStress::kName = "jsStress";
MTypeId JSStress::kId(0x00000007);

JSStress::JSStress() {}
JSStress::~JSStress() {}
void* JSStress::creator() { return new JSStress; }
MStatus JSStress::initialize()
{
	MStatus status;
	return MS::kSuccess;
}

MStatus JSStress::deform(MDataBlock &data, MItGeometry &iterGeo, const MMatrix &toWorldMatrix, unsigned int indexGeo)
{
	MStatus status;
	return MS::kSuccess;
}