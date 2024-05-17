exsltFreeFuncResultPreComp (exsltFuncResultPreComp *comp) {
    if (comp == NULL)
	return;

    if (comp->select != NULL)
	xmlXPathFreeCompExpr (comp->select);
    if (comp->nsList != NULL)
        xmlFree(comp->nsList);
    xmlFree(comp);
}
