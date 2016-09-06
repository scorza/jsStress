#include "jsStressCmd.h"

const char* JSStressCmd::kName = "jsStressCmd";

JSStressCmd::JSStressCmd() {};
void* JSStressCmd::creator() { return new JSStressCmd; }

MSyntax JSStressCmd::newSyntax()
{
	MSyntax syntax;
	return syntax;
}
