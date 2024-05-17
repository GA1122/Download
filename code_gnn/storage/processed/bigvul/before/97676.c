xmlXPathFreeCompExpr(xmlXPathCompExprPtr comp)
{
    xmlXPathStepOpPtr op;
    int i;

    if (comp == NULL)
        return;
    if (comp->dict == NULL) {
	for (i = 0; i < comp->nbStep; i++) {
	    op = &comp->steps[i];
	    if (op->value4 != NULL) {
		if (op->op == XPATH_OP_VALUE)
		    xmlXPathFreeObject(op->value4);
		else
		    xmlFree(op->value4);
	    }
	    if (op->value5 != NULL)
		xmlFree(op->value5);
	}
    } else {
	for (i = 0; i < comp->nbStep; i++) {
	    op = &comp->steps[i];
	    if (op->value4 != NULL) {
		if (op->op == XPATH_OP_VALUE)
		    xmlXPathFreeObject(op->value4);
	    }
	}
        xmlDictFree(comp->dict);
    }
    if (comp->steps != NULL) {
        xmlFree(comp->steps);
    }
#ifdef DEBUG_EVAL_COUNTS
    if (comp->string != NULL) {
        xmlFree(comp->string);
    }
#endif
#ifdef XPATH_STREAMING
    if (comp->stream != NULL) {
        xmlFreePatternList(comp->stream);
    }
#endif
    if (comp->expr != NULL) {
        xmlFree(comp->expr);
    }

    xmlFree(comp);
}
