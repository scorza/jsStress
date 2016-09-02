#pragma once
#include "common.h"

class JSStress : public MPxDeformerNode
{
public:
	JSStress();
	virtual ~JSStress();
	static void* creator();
	static MStatus initialize();

	virtual MStatus deform(MDataBlock &data, 
						   MItGeometry &iterGeo, 
						   const MMatrix &toWorldMatrix,
						   unsigned int indexGeo);

	// Static member variables
	static const char* kName;
	static MTypeId kId;

	// Static attribute variables
	static MObject aBulgeAmount;
	static MObject aRedOutput;
	static MObject aGreenOutput;
	static MObject aBlueOutput;
	static MObject aColorOutput;

private:
	int count_;
	std::vector<MDoubleArray> edgeLengths_;
	std::vector<MDoubleArray> initEdgeLengths_;
	MDoubleArray diffs_;
};