xsltDebuggerStartSequenceConstructor(xsltTransformContextPtr ctxt,
				     xmlNodePtr contextNode,
				     xmlNodePtr list,
				     xsltTemplatePtr templ,
				     int *addCallResult)
{
    xmlNodePtr debugedNode = NULL;

    if (ctxt->debugStatus != XSLT_DEBUG_NONE) {
        if (templ) {
            *addCallResult = xslAddCall(templ, templ->elem);
        } else {
            *addCallResult = xslAddCall(NULL, list);
        }
        switch (ctxt->debugStatus) {
            case XSLT_DEBUG_RUN_RESTART:
            case XSLT_DEBUG_QUIT:
                if (*addCallResult)
                    xslDropCall();
                return(NULL);
        }
        if (templ) {
            xslHandleDebugger(templ->elem, contextNode, templ, ctxt);
            debugedNode = templ->elem;
        } else if (list) {
            xslHandleDebugger(list, contextNode, templ, ctxt);
            debugedNode = list;
        } else if (ctxt->inst) {
            xslHandleDebugger(ctxt->inst, contextNode, templ, ctxt);
            debugedNode = ctxt->inst;
        }
    }
    return(debugedNode);
}
