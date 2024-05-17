xmlXPathRewriteDOSExpression(xmlXPathCompExprPtr comp, xmlXPathStepOpPtr op)
{
     
    if (op->ch1 != -1) {
	if ((op->op == XPATH_OP_COLLECT  ) &&
	    ((xmlXPathAxisVal) op->value == AXIS_CHILD  ) &&
	    ((xmlXPathTestVal) op->value2 == NODE_TEST_NAME  ) &&
	    ((xmlXPathTypeVal) op->value3 == NODE_TYPE_NODE  ))
	{
	     
	    xmlXPathStepOpPtr prevop = &comp->steps[op->ch1];

	    if ((prevop->op == XPATH_OP_COLLECT  ) &&
		(prevop->ch1 != -1) &&
		((xmlXPathAxisVal) prevop->value ==
		    AXIS_DESCENDANT_OR_SELF) &&
		(prevop->ch2 == -1) &&
		((xmlXPathTestVal) prevop->value2 == NODE_TEST_TYPE) &&
		((xmlXPathTypeVal) prevop->value3 == NODE_TYPE_NODE) &&
		(comp->steps[prevop->ch1].op == XPATH_OP_ROOT))
	    {
		 
		op->ch1 = prevop->ch1;
		op->rewriteType = XP_REWRITE_DOS_CHILD_ELEM;
	    }
	}
	if (op->ch1 != -1)
	    xmlXPathRewriteDOSExpression(comp, &comp->steps[op->ch1]);
    }
    if (op->ch2 != -1)
	xmlXPathRewriteDOSExpression(comp, &comp->steps[op->ch2]);
}
