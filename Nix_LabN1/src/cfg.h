#pragma once
#include "Array.h"
#include "ValueType.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdbool.h>

static int cfg_id_counter = 0;
static int opNode_id_counter = 0;

#define MAX_NODES 1000

#define MAX_VERTICES 1000

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
	OT_CONST,
	OT_ARRAY,
} OpType;

typedef struct OpNode {
	int id;

	struct AstNode* ast;
	char* value;
	struct Array* args;

	enum OpType opType;
	ValueType* valueType;
} OpNode;

typedef struct CfgNode {
	int id;
	struct AstNode* ast;
	char* label;
	struct OpNode* opTree;
	struct CfgNode* uncondJump;
	struct CfgNode* condJump;
} CfgNode;

// ������� ��� BFS
typedef struct {
	CfgNode* items[MAX_VERTICES];
	int front;
	int rear;
} Queue;

void initQueue(Queue* q);

bool isEmpty(Queue* q);

void enqueue(Queue* q, CfgNode* value);

CfgNode* dequeue(Queue* q);

CfgNode* createCfgNode(char* label, AstNode* ast);

OpNode* createOpNode(char* value, enum OpType opType, AstNode* ast);

int generateCfgDGML(CfgNode* root, const char* filename);

void addNodesAndLinksCfg(xmlNodePtr nodes, xmlNodePtr links, CfgNode* node);

int generateOpTreeDGML(OpNode* root, const char* filename);

char* strCpy(char* src);

void TraverseCfg(CfgNode* root, int(*value)(OpNode*, char*));

void addNodesAndLinksOpTree(xmlNodePtr nodes, xmlNodePtr links, OpNode* node);

