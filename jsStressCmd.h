#pragma once
#include "common.h"

class JSStressCmd : public MPxCommand
{
public:
	JSStressCmd();
	static void* creator();
	static MSyntax newSyntax();

	virtual MStatus doIt(const MArgList &argList);
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	virtual bool isUndoable() const;

	static const char* kName;

};