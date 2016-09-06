#pragma once
#include "common.h"

class JSStressCmd : public MPxCommand
{
public:
	// Built in funcs
	JSStressCmd();
	static void* creator();
	static MSyntax newSyntax();

	virtual MStatus doIt(const MArgList &argList);
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	virtual bool isUndoable() const;

	// Static name var
	static const char* kName;

	// Command flags
	static const char* kNameFlagShort;
	static const char* kNameFlagLong;

private:
	// Funcs
	MStatus getShapeNode(MDagPath &path);
	MStatus getLastNode(MObject &oStressNode);
	MStatus createRefMesh(MDagPath &path);

	// Vars
	MDGModifier dgMod_;
	MDagPath pathOrig_;
	MString nameArg_;
	MObject stressNode_;
};