#include "common.h"
#include "jsStress.h"

const char* JSStress::kName = "jsStress";
MTypeId JSStress::kId(0x00000007);

MObject JSStress::aRefMesh;
MObject JSStress::aBulgeAmount;
MObject JSStress::aColorOutput;

JSStress::JSStress() : count_(0) {}
JSStress::~JSStress() {}
void* JSStress::creator() { return new JSStress; }
MStatus JSStress::initialize()
{
	MStatus status;

	MFnNumericAttribute fnNumeric;
	MFnTypedAttribute fnTyped;
	
	aRefMesh = fnTyped.create("referenceMesh", "referenceMesh", MFnData::kMesh);
	fnTyped.setHidden(true);
	addAttribute(aRefMesh);

	aBulgeAmount = fnNumeric.create("bulgeAmount", "bulgeAmount", MFnNumericData::kFloat, 0.5);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	addAttribute(aBulgeAmount);

	aColorOutput = fnNumeric.create("colorOutput", "colorOutput", MFnNumericData::k3Double);
	fnNumeric.setKeyable(false);
	fnNumeric.setStorable(false);
	fnNumeric.setMin(0.0);
	fnNumeric.setMax(1.0);
	addAttribute(aColorOutput);

	attributeAffects(aRefMesh, outputGeom);
	attributeAffects(aBulgeAmount, outputGeom);

	attributeAffects(aRefMesh, aColorOutput);
	attributeAffects(inputGeom, aColorOutput);
	attributeAffects(aBulgeAmount, aColorOutput);

	return MS::kSuccess;
}

MStatus JSStress::deform(MDataBlock &data, MItGeometry &iterGeo, const MMatrix &toWorldMatrix, unsigned int indexGeo)
{
	MStatus status;

	// Get basic input values
	float env = data.inputValue(envelope).asFloat();
	float bulgeAmount = data.inputValue(aBulgeAmount).asFloat();
	
	// Get input geom as mobject
	MArrayDataHandle hArrayInput = data.inputArrayValue(input);
	hArrayInput.jumpToElement(indexGeo);
	MDataHandle hElementInput = hArrayInput.inputValue();
	MDataHandle hInput = hElementInput.child(inputGeom);
	MObject inputGeo = hInput.asMesh();

	MPointArray pts, outPts;
	iterGeo.allPositions(pts, MSpace::kWorld);
	edgeLengths_.resize(pts.length());
	diffs_.setLength(pts.length());
	outPts.setLength(pts.length());

	MFnMesh fnMesh(inputGeo);
	MFloatVectorArray normals;
	fnMesh.getVertexNormals(false, normals);

	for (unsigned int i = 0; i < pts.length(); ++i)
	{
		MObject pt = iterGeo.currentItem();
		
		MItMeshEdge iterEdge(inputGeo, pt);
		int numEdges;
		iterEdge.numConnectedEdges(numEdges);
		edgeLengths_[i].setLength(numEdges);
		for (unsigned int j = 0; j < numEdges; ++j)
		{
			double length;
			status = iterEdge.getLength(length, MSpace::kWorld);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			edgeLengths_[i][j] = length;
			iterEdge.next();
		}
		iterGeo.next();
	}
	iterGeo.reset();


	if (count_ == 0)
	{
		initEdgeLengths_ = edgeLengths_;
		++count_;

		return MS::kSuccess;
	}

	for (unsigned int i = 0; i < pts.length(); ++i)
	{


		double diff = 0.0;
		for (unsigned int j = 0; j < edgeLengths_[i].length(); ++j)
		{
			diff += edgeLengths_[i][j] - initEdgeLengths_[i][j];
		}
		if (i == 119)
		{
			cout << "initial edge lengths are " << initEdgeLengths_[i][0] << ", " << initEdgeLengths_[i][1] << ", " << initEdgeLengths_[i][2] << ", " << endl;
			cout << "current edge lengths are " << edgeLengths_[i][0] << ", " << edgeLengths_[i][1] << ", " << edgeLengths_[i][2] << ", " << endl;
			cout << "diff for vert 119 is ... " << diff << endl;
		}

		// diff *= (1 / edgeLengths_[i].length());
		diffs_[i] = diff;
	}

	for (unsigned int i = 0; i < pts.length(); ++i)
	{
		MPoint norm = normals[i];
		if (i == 119)
		{
			cout << " norm for vert 119 == " << norm.x << " " << norm.y << " " << norm.z << endl << endl;
		}
		MPoint pt = pts[i];
		MPoint outPt = pt;
		outPt += norm * diffs_[i];
		outPt = outPt + ((outPt - pt) * bulgeAmount * env);
		outPts[i] = outPt;
	}
	
	iterGeo.setAllPositions(outPts);

	
	return MS::kSuccess;
}