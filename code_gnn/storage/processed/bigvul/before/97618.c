xmlXPathCompStep(xmlXPathParserContextPtr ctxt) {
#ifdef LIBXML_XPTR_ENABLED
    int rangeto = 0;
    int op2 = -1;
#endif

    SKIP_BLANKS;
    if ((CUR == '.') && (NXT(1) == '.')) {
	SKIP(2);
	SKIP_BLANKS;
	PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_PARENT,
		    NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
    } else if (CUR == '.') {
	NEXT;
	SKIP_BLANKS;
    } else {
	xmlChar *name = NULL;
	const xmlChar *prefix = NULL;
	xmlXPathTestVal test = (xmlXPathTestVal) 0;
	xmlXPathAxisVal axis = (xmlXPathAxisVal) 0;
	xmlXPathTypeVal type = (xmlXPathTypeVal) 0;
	int op1;

	 
#ifdef LIBXML_XPTR_ENABLED
	if (ctxt->xptr) {
	    name = xmlXPathParseNCName(ctxt);
	    if ((name != NULL) && (xmlStrEqual(name, BAD_CAST "range-to"))) {
                op2 = ctxt->comp->last;
		xmlFree(name);
		SKIP_BLANKS;
		if (CUR != '(') {
		    XP_ERROR(XPATH_EXPR_ERROR);
		}
		NEXT;
		SKIP_BLANKS;

		xmlXPathCompileExpr(ctxt, 1);
		 
		CHECK_ERROR;

		SKIP_BLANKS;
		if (CUR != ')') {
		    XP_ERROR(XPATH_EXPR_ERROR);
		}
		NEXT;
		rangeto = 1;
		goto eval_predicates;
	    }
	}
#endif
	if (CUR == '*') {
	    axis = AXIS_CHILD;
	} else {
	    if (name == NULL)
		name = xmlXPathParseNCName(ctxt);
	    if (name != NULL) {
		axis = xmlXPathIsAxisName(name);
		if (axis != 0) {
		    SKIP_BLANKS;
		    if ((CUR == ':') && (NXT(1) == ':')) {
			SKIP(2);
			xmlFree(name);
			name = NULL;
		    } else {
			 
			axis = AXIS_CHILD;
		    }
		} else {
		    axis = AXIS_CHILD;
		}
	    } else if (CUR == '@') {
		NEXT;
		axis = AXIS_ATTRIBUTE;
	    } else {
		axis = AXIS_CHILD;
	    }
	}

	CHECK_ERROR;

	name = xmlXPathCompNodeTest(ctxt, &test, &type, &prefix, name);
	if (test == 0)
	    return;

        if ((prefix != NULL) && (ctxt->context != NULL) &&
	    (ctxt->context->flags & XML_XPATH_CHECKNS)) {
	    if (xmlXPathNsLookup(ctxt->context, prefix) == NULL) {
		xmlXPathErr(ctxt, XPATH_UNDEF_PREFIX_ERROR);
	    }
	}
#ifdef DEBUG_STEP
	xmlGenericError(xmlGenericErrorContext,
		"Basis : computing new set\n");
#endif

#ifdef DEBUG_STEP
	xmlGenericError(xmlGenericErrorContext, "Basis : ");
	if (ctxt->value == NULL)
	    xmlGenericError(xmlGenericErrorContext, "no value\n");
	else if (ctxt->value->nodesetval == NULL)
	    xmlGenericError(xmlGenericErrorContext, "Empty\n");
	else
	    xmlGenericErrorContextNodeSet(stdout, ctxt->value->nodesetval);
#endif

#ifdef LIBXML_XPTR_ENABLED
eval_predicates:
#endif
	op1 = ctxt->comp->last;
	ctxt->comp->last = -1;

	SKIP_BLANKS;
	while (CUR == '[') {
	    xmlXPathCompPredicate(ctxt, 0);
	}

#ifdef LIBXML_XPTR_ENABLED
	if (rangeto) {
	    PUSH_BINARY_EXPR(XPATH_OP_RANGETO, op2, op1, 0, 0);
	} else
#endif
	    PUSH_FULL_EXPR(XPATH_OP_COLLECT, op1, ctxt->comp->last, axis,
			   test, type, (void *)prefix, (void *)name);

    }
#ifdef DEBUG_STEP
    xmlGenericError(xmlGenericErrorContext, "Step : ");
    if (ctxt->value == NULL)
	xmlGenericError(xmlGenericErrorContext, "no value\n");
    else if (ctxt->value->nodesetval == NULL)
	xmlGenericError(xmlGenericErrorContext, "Empty\n");
    else
	xmlGenericErrorContextNodeSet(xmlGenericErrorContext,
		ctxt->value->nodesetval);
#endif
}
