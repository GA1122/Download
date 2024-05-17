exsltFuncRegister (void) {
    xsltRegisterExtModuleFull (EXSLT_FUNCTIONS_NAMESPACE,
		       (xsltExtInitFunction) exsltFuncInit,
		       (xsltExtShutdownFunction) exsltFuncShutdown,
		       (xsltStyleExtInitFunction) exsltFuncStyleInit,
		       (xsltStyleExtShutdownFunction) exsltFuncStyleShutdown);

    xsltRegisterExtModuleTopLevel ((const xmlChar *) "function",
				   EXSLT_FUNCTIONS_NAMESPACE,
				   exsltFuncFunctionComp);
    xsltRegisterExtModuleElement ((const xmlChar *) "result",
			  EXSLT_FUNCTIONS_NAMESPACE,
			  (xsltPreComputeFunction)exsltFuncResultComp,
			  (xsltTransformFunction) exsltFuncResultElem);
}
