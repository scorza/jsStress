#include "common.h"
#include "jsStress.h"
#include "jsStressCmd.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject mobj)
{
	MStatus status;
	MFnPlugin fnPlugin(mobj, "James Sumner III", "0.1", "2016", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerNode(JSStress::kName, JSStress::kId, JSStress::creator, JSStress::initialize, MPxNode::kDeformerNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return MS::kSuccess;

}

MStatus uninitializePlugin(MObject mobj)
{
	MStatus status;
	MFnPlugin fnPlugin(mobj);
	
	status = fnPlugin.deregisterNode(JSStress::kId);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return MS::kSuccess;

}