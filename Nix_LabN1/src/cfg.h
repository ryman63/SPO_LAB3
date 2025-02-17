#pragma once
#include "Array.h"
#include "ValueType.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdbool.h>

static int cfg_id_counter = 0;
static int opNode_id_counter = 0;

#define MAX_NODES 1000

static bool visited[MAX_NODES] = { false };
static bool visited2[MAX_NODES] = { false };

typedef enum OpType {
	OT_NON,
	OT_PLACE,
	OT_LITERAL,
	OT_INDEX,
	OT_WRITE,
	OT_READ,
	OT_BINARY,
	OT_UNARY,
	OT_CALL,
	OT_CONST
};

typedef struct OpNode {
	int id;
	char* value;
	struct Array* args;

	enum OpType opType;
	enum ValueType valueType;
} OpNode;

typedef struct CfgNode {
	int id;
	int line;
	char* label;
	struct OpNode* opTree;
	struct CfgNode* uncondJump;
	struct CfgNode* condJump;
} CfgNode;

CfgNode* createCfgNode(char* label, int line);

OpNode* createOpNode(char* value, enum OpType opType);

int generateCfgDGML(CfgNode* root, const char* filename);

void addNodesAndLinksCfg(xmlNodePtr nodes, xmlNodePtr links, CfgNode* node);

int generateOpTreeDGML(OpNode* root, const char* filename);

char* strCpy(char* src);

void TraverseCfg(CfgNode* root, int(*value)(OpNode*, char*));

void addNodesAndLinksOpTree(xmlNodePtr nodes, xmlNodePtr links, OpNode* node);

