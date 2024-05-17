xmlXPathDebugDumpStepAxis(xmlXPathStepOpPtr op,
			  int nbNodes)
{
    xmlGenericError(xmlGenericErrorContext, "new step : ");
    switch (op->value) {
        case AXIS_ANCESTOR:
            xmlGenericError(xmlGenericErrorContext, "axis 'ancestors' ");
            break;
        case AXIS_ANCESTOR_OR_SELF:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'ancestors-or-self' ");
            break;
        case AXIS_ATTRIBUTE:
            xmlGenericError(xmlGenericErrorContext, "axis 'attributes' ");
            break;
        case AXIS_CHILD:
            xmlGenericError(xmlGenericErrorContext, "axis 'child' ");
            break;
        case AXIS_DESCENDANT:
            xmlGenericError(xmlGenericErrorContext, "axis 'descendant' ");
            break;
        case AXIS_DESCENDANT_OR_SELF:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'descendant-or-self' ");
            break;
        case AXIS_FOLLOWING:
            xmlGenericError(xmlGenericErrorContext, "axis 'following' ");
            break;
        case AXIS_FOLLOWING_SIBLING:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'following-siblings' ");
            break;
        case AXIS_NAMESPACE:
            xmlGenericError(xmlGenericErrorContext, "axis 'namespace' ");
            break;
        case AXIS_PARENT:
            xmlGenericError(xmlGenericErrorContext, "axis 'parent' ");
            break;
        case AXIS_PRECEDING:
            xmlGenericError(xmlGenericErrorContext, "axis 'preceding' ");
            break;
        case AXIS_PRECEDING_SIBLING:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'preceding-sibling' ");
            break;
        case AXIS_SELF:
            xmlGenericError(xmlGenericErrorContext, "axis 'self' ");
            break;
    }
    xmlGenericError(xmlGenericErrorContext,
	" context contains %d nodes\n", nbNodes);
    switch (op->value2) {
        case NODE_TEST_NONE:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for none !!!\n");
            break;
        case NODE_TEST_TYPE:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for type %d\n", op->value3);
            break;
        case NODE_TEST_PI:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for PI !!!\n");
            break;
        case NODE_TEST_ALL:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for *\n");
            break;
        case NODE_TEST_NS:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for namespace %s\n",
                            op->value5);
            break;
        case NODE_TEST_NAME:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for name %s\n", op->value5);
            if (op->value4)
                xmlGenericError(xmlGenericErrorContext,
                                "           with namespace %s\n", op->value4);
            break;
    }
    xmlGenericError(xmlGenericErrorContext, "Testing : ");
}
