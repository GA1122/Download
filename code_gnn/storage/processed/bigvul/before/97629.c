xmlXPathCompileExpr(xmlXPathParserContextPtr ctxt, int sort) {
    xmlXPathCompAndExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == 'o') && (NXT(1) == 'r')) {
	int op1 = ctxt->comp->last;
        SKIP(2);
	SKIP_BLANKS;
        xmlXPathCompAndExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_OR, op1, ctxt->comp->last, 0, 0);
	SKIP_BLANKS;
    }
    if ((sort) && (ctxt->comp->steps[ctxt->comp->last].op != XPATH_OP_VALUE)) {
	 
	 
	PUSH_UNARY_EXPR(XPATH_OP_SORT, ctxt->comp->last , 0, 0);
    }
}
