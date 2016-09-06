#pragma once
// C++ INCLUDES
#include <vector>

// GENERAL INCLUDES
#include <maya/MArgDatabase.h>
#include <maya/MDagPath.h>
#include <maya/MDoubleArray.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MSyntax.h>

// ITERATOR INCLUDES
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshVertex.h>

// MFN ATTR INCLUDES
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

// MFN INCLUDES
#include <maya/MFnMesh.h>

// MPX INCLUDES
#include <maya/MPxNode.h>
#include <maya/MPxCommand.h>
#include <maya/MPxDeformerNode.h>