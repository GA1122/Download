xsltEvalOneUserParam(xsltTransformContextPtr ctxt,
		     const xmlChar * name,
		     const xmlChar * value) {
    return xsltProcessUserParamInternal(ctxt, name, value,
		                        1  );
}
