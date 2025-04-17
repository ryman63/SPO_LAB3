#include "cfg.h"

CfgNode* createCfgNode(char* label, AstNode* ast) {
	CfgNode* createNode = malloc(sizeof(CfgNode));

	createNode->label = label;
	createNode->ast = ast;

	createNode->condJump = NULL;
	createNode->uncondJump = NULL;
	createNode->opTree = NULL;

	createNode->id = cfg_id_counter;
	cfg_id_counter++;

	return createNode;
}

OpNode* createOpNode(char* value, enum OpType opType, AstNode* ast)
{
	OpNode* createNode = malloc(sizeof(OpNode));

	createNode->opType = opType;
	createNode->value = value;
	createNode->ast = ast;
	createNode->args = buildArray(sizeof(OpNode), 2);
	createNode->id = opNode_id_counter;

	opNode_id_counter++;
	
	return createNode;
}

void addNodesAndLinksCfg(xmlNodePtr nodes, xmlNodePtr links, CfgNode* node) {

	if (visited[node->id])
		return;

	visited[node->id] = true;

	// ������� ������� Node ��� �������� ����
	char id_str[12];
	snprintf(id_str, 12, "%d", node->id); // ����������� id � ������
	xmlNodePtr node_elem = xmlNewChild(nodes, NULL, BAD_CAST "Node", NULL);
	xmlNewProp(node_elem, BAD_CAST "Id", BAD_CAST id_str);
	xmlNewProp(node_elem, BAD_CAST "Label", BAD_CAST node->label);

	// ��������, ��� � ���� ���� �������� ��������
	if (node->condJump) {
		CfgNode* child = node->condJump;

		// ������� ������� Link ��� ����� ����� ����� � ��� �������� �����
		char child_id_str[12];
		snprintf(child_id_str, 12, "%d", child->id);
		xmlNodePtr link_elem = xmlNewChild(links, NULL, BAD_CAST "Link", NULL);
		xmlNewProp(link_elem, BAD_CAST "Source", BAD_CAST id_str);
		xmlNewProp(link_elem, BAD_CAST "Target", BAD_CAST child_id_str);

		// ���������� ������������ �������� ����
		addNodesAndLinksCfg(nodes, links, child);
	}
	if (node->uncondJump) {
		CfgNode* child = node->uncondJump;

		// ������� ������� Link ��� ����� ����� ����� � ��� �������� �����
		char child_id_str[12];
		snprintf(child_id_str, 12, "%d", child->id);
		xmlNodePtr link_elem = xmlNewChild(links, NULL, BAD_CAST "Link", NULL);
		xmlNewProp(link_elem, BAD_CAST "Source", BAD_CAST id_str);
		xmlNewProp(link_elem, BAD_CAST "Target", BAD_CAST child_id_str);

		// ���������� ������������ �������� ����
		addNodesAndLinksCfg(nodes, links, child);
	}
}

int generateCfgDGML(CfgNode* root, const char* filename) {
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_elem = xmlNewNode(NULL, BAD_CAST "DirectedGraph");
	xmlNewProp(root_elem, BAD_CAST "xmlns", BAD_CAST "http://schemas.microsoft.com/vs/2009/dgml");
	xmlDocSetRootElement(doc, root_elem);

	// ������� �������� Nodes � Links
	xmlNodePtr nodes_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Nodes", NULL);
	xmlNodePtr links_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Links", NULL);

	// ��������� ���� � �����
	addNodesAndLinksCfg(nodes_elem, links_elem, root);

	// ��������� DGML ����
	int result = xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return result;
}

void addNodesAndLinksOpTree(xmlNodePtr nodes, xmlNodePtr links, OpNode* node) {
	// ������� ������� Node ��� �������� ����
	char id_str[12];
	snprintf(id_str, 12, "%d", node->id); // ����������� id � ������
	xmlNodePtr node_elem = xmlNewChild(nodes, NULL, BAD_CAST "Node", NULL);
	xmlNewProp(node_elem, BAD_CAST "Id", BAD_CAST id_str);
	xmlNewProp(node_elem, BAD_CAST "Label", BAD_CAST node->value);

	// ��������, ��� � ���� ���� �������� ��������
	if (node->args != NULL) {
		for (size_t i = 0; i < node->args->size; i++) {
			AstNode* child = getItem(node->args, i);
			if (child == NULL) continue;

			// ������� ������� Link ��� ����� ����� ����� � ��� �������� �����
			char child_id_str[12];
			snprintf(child_id_str, 12, "%d", child->id);
			xmlNodePtr link_elem = xmlNewChild(links, NULL, BAD_CAST "Link", NULL);
			xmlNewProp(link_elem, BAD_CAST "Source", BAD_CAST id_str);
			xmlNewProp(link_elem, BAD_CAST "Target", BAD_CAST child_id_str);

			// ���������� ������������ �������� ����
			addNodesAndLinksOpTree(nodes, links, child);
		}
	}
}

int generateOpTreeDGML(OpNode* root, const char* filename) {
	char saveDirectory[64] = "OpTreeDGML\\";
	strcat_s(saveDirectory, 64, filename);
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_elem = xmlNewNode(NULL, BAD_CAST "DirectedGraph");
	xmlNewProp(root_elem, BAD_CAST "xmlns", BAD_CAST "http://schemas.microsoft.com/vs/2009/dgml");
	xmlDocSetRootElement(doc, root_elem);

	// ������� �������� Nodes � Links
	xmlNodePtr nodes_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Nodes", NULL);
	xmlNodePtr links_elem = xmlNewChild(root_elem, NULL, BAD_CAST "Links", NULL);

	// ��������� ���� � �����
	addNodesAndLinksOpTree(nodes_elem, links_elem, root);

	// ��������� DGML ����
	int result = xmlSaveFormatFileEnc(saveDirectory, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return result;
}

char* strCpy(char* src) {
	size_t tokenLen = strlen(src);
	char* tokenCopy = malloc(tokenLen + 1);

#ifdef _WIN32
	strcpy_s(tokenCopy, tokenLen + 1, src);
#else
	strcpy(tokenCopy, src);
#endif

	tokenCopy[tokenLen] = '\0';
	return tokenCopy;
}

void TraverseCfg(CfgNode* root, int(*value)(OpNode*, const char*)) {
	if (visited2[root->id])
		return;

	visited2[root->id] = true;

	if (root) {
		if (root->condJump) TraverseCfg(root->condJump, value);
		if (root->uncondJump) TraverseCfg(root->uncondJump, value);
	}
	if (root->opTree)
	{
		char fileName[64];
		strcpy_s(fileName, 64, root -> label);
		strcat_s(fileName, 64, "_");
		char buff[8];
		_itoa_s(root->ast->line, buff, 8, 10);
		strcat_s(fileName, 64, buff);
		strcat_s(fileName, 64, ".dgml");
		value(root->opTree, fileName);
	}
}