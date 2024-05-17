exsltFuncRegisterFunc (exsltFuncFunctionData *data,
		       xsltTransformContextPtr ctxt,
		       const xmlChar *URI, const xmlChar *name,
		       ATTRIBUTE_UNUSED const xmlChar *ignored) {
    if ((data == NULL) || (ctxt == NULL) || (URI == NULL) || (name == NULL))
	return;

    xsltGenericDebug(xsltGenericDebugContext,
		     "exsltFuncRegisterFunc: register {%s}%s\n",
		     URI, name);
    xsltRegisterExtFunction(ctxt, name, URI,
			    exsltFuncFunctionFunction);
}
