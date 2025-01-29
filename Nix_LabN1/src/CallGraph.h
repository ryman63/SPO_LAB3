#pragma once

#include "ProgramUnit.h"

static int callGraph_id_counter = 0;

#define MAX_NODES 1000

static bool visited_cfgNodes[MAX_NODES] = { false };

typedef struct CallGraphNode {
	int id;
	ProgramUnit* unit;
	Array* children;
} CallGraphNode;


CallGraphNode* createCGNode(ProgramUnit* unit);

CallGraphNode* createCGNodeOfName(const char* name);

CallGraphNode* buildCallGraph(Array* programUnitStorage);

Array* VisitProgramUnit(ProgramUnit* unit, Array* programUnitStorage);

ProgramUnit* searchUnit(char* unitName, Array* programUnitStorage);

void VisitCfg(CfgNode* cfg, Array* callOps);

OpNode* findOp(OpNode* opHead);

void addNodesAndLinksCallGraph(xmlNodePtr nodes, xmlNodePtr links, CallGraphNode* node);

int generateCallGraphDGML(CallGraphNode* root, const char* filename);