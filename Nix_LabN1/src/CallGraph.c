#include "CallGraph.h"

CallGraphNode* createCGNode(ProgramUnit* unit) {
	CallGraphNode* createNode = (CallGraphNode*)malloc(sizeof(CallGraphNode));
	ProgramUnit* copyUnit = malloc(sizeof(ProgramUnit));
	copyUnit->cfg = unit->cfg;
	copyUnit->funcSignature = unit->funcSignature;
	copyUnit->sourceFile = unit->sourceFile;

	createNode->unit = copyUnit;

	createNode->children = NULL;
	createNode->id = callGraph_id_counter;
	callGraph_id_counter++;
	return createNode;
}

CallGraphNode* createCGNodeOfName(const char* name) {
	CallGraphNode* createNode = (CallGraphNode*)malloc(sizeof(CallGraphNode));
	ProgramUnit* unit = malloc(sizeof(ProgramUnit));
	unit->funcSignature = createFuncSignature();
	unit->funcSignature->name = strCpy(name);
	createNode->unit = unit;
	createNode->children = NULL;
	createNode->id = callGraph_id_counter;
	callGraph_id_counter++;
	return createNode;
}

CallGraphNode* buildCallGraph(Array* programUnitStorage) {
	ProgramUnit* mainUnit = searchUnit("main", programUnitStorage);
	if (mainUnit)
	{
		CallGraphNode* mainCall = createCGNode(mainUnit);
		
		int i = 0;

		mainCall->children = VisitProgramUnit(mainUnit, programUnitStorage);

		return mainCall;
	}
	else
	{
		// обработка ошибки

		return NULL;
	}
}

Array* VisitProgramUnit(ProgramUnit* unit, Array* programUnitStorage) {
	Array* resultNodes = buildArray(sizeof(CallGraphNode), 8);
	Array* callOps = buildArray(sizeof(OpNode), 4);
	VisitCfg(unit->cfg, callOps);
	for (size_t j = 0; j < callOps->size; j++) {
		OpNode* currOpNode = getItem(callOps, j);
		OpNode* CallNameOp = getItem(currOpNode->args, 0);
		ProgramUnit* findUnit = searchUnit(CallNameOp->value, programUnitStorage);
		if (findUnit) {
			CallGraphNode* newCall = createCGNode(findUnit);
			newCall->children = VisitProgramUnit(findUnit, programUnitStorage);
			pushBack(resultNodes, newCall);
		}
		
		// если нашли вызов, но не нашли её реализацию то создаём пустой юнит
		if (CallNameOp && !findUnit) {
			CallGraphNode* newCall = createCGNodeOfName(CallNameOp->value);
			pushBack(resultNodes, newCall);
		}
		
	}
	freeArray(&callOps);

	return resultNodes;
}

ProgramUnit* searchUnit(char* unitName, Array* programUnitStorage) {
	for (size_t i = 0; i < programUnitStorage->size; i++) {
		ProgramUnit* currentUnit = getItem(programUnitStorage, i);
		if (!strcmp(currentUnit->funcSignature->name, unitName))
			return currentUnit;
	}
	return NULL;
}

void VisitCfg(CfgNode* cfg, Array* callOps) {
	if (visited_cfgNodes[cfg->id])
		return;

	visited_cfgNodes[cfg->id] = true;

	if (cfg->condJump)
		VisitCfg(cfg->condJump, callOps);
	if (cfg->uncondJump)
		VisitCfg(cfg->uncondJump, callOps);

	if (cfg->opTree) {
		OpNode* callOp = findOp(cfg->opTree);
		if (callOp)
			pushBack(callOps, callOp);
	}
}

OpNode* findOp(OpNode* opHead) {
	if (!strcmp(opHead->value, "call")) return opHead;

	for (size_t i = 0; i < opHead->args->size; i++) {
		return findOp(getItem(opHead->args, i));
	}

	return NULL;
}

void addNodesAndLinksCallGraph(xmlNodePtr nodes, xmlNodePtr links, CallGraphNode* node) {
	// Создаем элемент Node для текущего узла
	char id_str[12];
	snprintf(id_str, 12, "%d", node->id); // Преобразуем id в строку
	xmlNodePtr node_elem = xmlNewChild(nodes, NULL, BAD_CAST "Node", NULL);
	xmlNewProp(node_elem, BAD_CAST "Id", BAD_CAST id_str);
	xmlNewProp(node_elem, BAD_CAST "Label", BAD_CAST node->unit->funcSignature->name);

	// Проверка, что у узла есть дочерние элементы
	if (node->children != NULL) {
		for (size_t i = 0; i < node->children->size; i++) {
			AstNode* child = getItem(node->children, i);
			if (child == NULL) continue;

			// Создаем элемент Link для связи между узлом и его дочерним узлом
			char child_id_str[12];
			snprintf(child_id_str, 12, "%d", child->id);
			xmlNodePtr link_elem = xmlNewChild(links, NULL, BAD_CAST "Link", NULL);
			xmlNewProp(link_elem, BAD_CAST "Source", BAD_CAST id_str);
			xmlNewProp(link_elem, BAD_CAST "Target", BAD_CAST child_id_str);

			// Рекурсивно обрабатываем дочерний узел
			addNodesAndLinksCallGraph(nodes, links, child);
		}
	}
}

int generateCallGraphDGML(CallGraphNode* root, const char* filename) {
	char saveDirectory[64] = "CallGraph\\";
	strcat_s(saveDirectory, 64, filename);
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_elem = xmlNewNode(NULL, BAD_CAST "DirectedGraph");
	xmlNewProp(root_elem, BAD_CAST "xmlns", BAD_CAST "http://schemas.microsoft.com/vs/2009/dgml");
	xmlDocSetRootElement(doc, root_elem);

	// Создаем элементы Nodes и Links
	xmlNodePtr nodes_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Nodes", NULL);
	xmlNodePtr links_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Links", NULL);

	// Добавляем узлы и связи
	addNodesAndLinksCallGraph(nodes_elem, links_elem, root);

	// Сохраняем DGML файл
	int result = xmlSaveFormatFileEnc(saveDirectory, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return result;
}