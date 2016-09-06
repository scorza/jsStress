#include "jsStress.h"
#include "jsStressCmd.h"

const char* JSStressCmd::kName = "jsStressCmd";
const char* JSStressCmd::kNameFlagShort = "-n";
const char* JSStressCmd::kNameFlagLong = "-name";

JSStressCmd::JSStressCmd() {};
void* JSStressCmd::creator() { return new JSStressCmd; }

MSyntax JSStressCmd::newSyntax()
{
	MSyntax syntax;
	
	syntax.addFlag(kNameFlagShort, kNameFlagLong, MSyntax::kString);
	syntax.setObjectType(MSyntax::kSelectionList, 1);
	syntax.useSelectionAsDefault(true);

	syntax.enableEdit(false);
	syntax.enableQuery(false);

	return syntax;
}

MStatus JSStressCmd::doIt(const MArgList &argList)
{
	MStatus status;

	MArgDatabase argData(syntax(), argList, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MSelectionList mSel;
	argData.getObjects(mSel);
	mSel.getDagPath(0, pathOrig_);

	// Get command arguments
	if (argData.isFlagSet("-n"))
	{
		nameArg_ = argData.flagArgumentString("-n", 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	else { nameArg_ = "jsStress#";  }

	// Set command to execute on dgMod_
	MString command = "deformer -type jsStress -n \"" + nameArg_ + "\" ";
	command += pathOrig_.partialPathName().asChar();
	status = dgMod_.commandToExecute(command);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return redoIt();
}

MStatus JSStressCmd::redoIt()
{
	MStatus status;

	status = dgMod_.doIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = getLastNode(stressNode_);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}

MStatus JSStressCmd::undoIt()
{
	MStatus status;
	return MS::kSuccess;
}

bool JSStressCmd::isUndoable() const
{
	return true;
}

MStatus JSStressCmd::getShapeNode(MDagPath &path)
{
	MStatus status;

	if (path.node().hasFn(MFn::kMesh))
	{
		return MS::kSuccess;
	}
	unsigned int numShapes;
	status = path.numberOfShapesDirectlyBelow(numShapes);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (unsigned int i = 0; i < numShapes; ++i)
	{
		status = path.extendToShapeDirectlyBelow(i);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (!path.hasFn(MFn::kMesh))
		{
			path.pop();
			continue;
		}

		MFnDagNode fnDag(path);
		if (!fnDag.isIntermediateObject())
		{
			return MS::kSuccess;
		}
		path.pop();
	}
	return MS::kFailure;
}

MStatus JSStressCmd::getLastNode(MObject &oStressNode)
{
	MStatus status;

	MObject oOrig = pathOrig_.node();
	MItDependencyGraph iterDG(oOrig, MFn::kGeometryFilt,
							MItDependencyGraph::kUpstream,
							MItDependencyGraph::kDepthFirst,
							MItDependencyGraph::kNodeLevel, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	while (!iterDG.isDone())
	{
		oStressNode = iterDG.currentItem();
		MFnDependencyNode fnDep(oStressNode);
		if (fnDep.typeId() == JSStress::kId)
		{
			return MS::kSuccess;
		}
		iterDG.next();
	}

	return MS::kFailure;
}

MStatus JSStressCmd::createRefMesh(MDagPath &path)
{
	MStatus status;

	MStringArray duplicateReturn;

	MFnDependencyNode fnDep(stressNode_);
	MFnDagNode fnDag(pathOrig_);

	MGlobal::executeCommand("duplicate -rr -n " + fnDag.name() + "_jsStressBase" + fnDag.partialPathName(), duplicateReturn);


	return MS::kSuccess;
}