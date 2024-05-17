xsltGetXSLTElementTypeByNode(xsltCompilerCtxtPtr cctxt,
			     xmlNodePtr node)
{
    if ((node == NULL) || (node->type != XML_ELEMENT_NODE) ||
	(node->name == NULL))
	return(0);

    if (node->name[0] == 'a') {
	if (IS_XSLT_NAME(node, "apply-templates"))
	    return(XSLT_FUNC_APPLYTEMPLATES);
	else if (IS_XSLT_NAME(node, "attribute"))
	    return(XSLT_FUNC_ATTRIBUTE);
	else if (IS_XSLT_NAME(node, "apply-imports"))
	    return(XSLT_FUNC_APPLYIMPORTS);
	else if (IS_XSLT_NAME(node, "attribute-set"))
	    return(0);

    } else if (node->name[0] == 'c') {
	if (IS_XSLT_NAME(node, "choose"))
	    return(XSLT_FUNC_CHOOSE);
	else if (IS_XSLT_NAME(node, "copy"))
	    return(XSLT_FUNC_COPY);
	else if (IS_XSLT_NAME(node, "copy-of"))
	    return(XSLT_FUNC_COPYOF);
	else if (IS_XSLT_NAME(node, "call-template"))
	    return(XSLT_FUNC_CALLTEMPLATE);
	else if (IS_XSLT_NAME(node, "comment"))
	    return(XSLT_FUNC_COMMENT);

    } else if (node->name[0] == 'd') {
	if (IS_XSLT_NAME(node, "document"))
	    return(XSLT_FUNC_DOCUMENT);
	else if (IS_XSLT_NAME(node, "decimal-format"))
	    return(0);

    } else if (node->name[0] == 'e') {
	if (IS_XSLT_NAME(node, "element"))
	    return(XSLT_FUNC_ELEMENT);

    } else if (node->name[0] == 'f') {
	if (IS_XSLT_NAME(node, "for-each"))
	    return(XSLT_FUNC_FOREACH);
	else if (IS_XSLT_NAME(node, "fallback"))
	    return(XSLT_FUNC_FALLBACK);

    } else if (*(node->name) == 'i') {
	if (IS_XSLT_NAME(node, "if"))
	    return(XSLT_FUNC_IF);
	else if (IS_XSLT_NAME(node, "include"))
	    return(0);
	else if (IS_XSLT_NAME(node, "import"))
	    return(0);

    } else if (*(node->name) == 'k') {
	if (IS_XSLT_NAME(node, "key"))
	    return(0);

    } else if (*(node->name) == 'm') {
	if (IS_XSLT_NAME(node, "message"))
	    return(XSLT_FUNC_MESSAGE);

    } else if (*(node->name) == 'n') {
	if (IS_XSLT_NAME(node, "number"))
	    return(XSLT_FUNC_NUMBER);
	else if (IS_XSLT_NAME(node, "namespace-alias"))
	    return(0);

    } else if (*(node->name) == 'o') {
	if (IS_XSLT_NAME(node, "otherwise"))
	    return(XSLT_FUNC_OTHERWISE);
	else if (IS_XSLT_NAME(node, "output"))
	    return(0);

    } else if (*(node->name) == 'p') {
	if (IS_XSLT_NAME(node, "param"))
	    return(XSLT_FUNC_PARAM);
	else if (IS_XSLT_NAME(node, "processing-instruction"))
	    return(XSLT_FUNC_PI);
	else if (IS_XSLT_NAME(node, "preserve-space"))
	    return(0);

    } else if (*(node->name) == 's') {
	if (IS_XSLT_NAME(node, "sort"))
	    return(XSLT_FUNC_SORT);
	else if (IS_XSLT_NAME(node, "strip-space"))
	    return(0);
	else if (IS_XSLT_NAME(node, "stylesheet"))
	    return(0);

    } else if (node->name[0] == 't') {
	if (IS_XSLT_NAME(node, "text"))
	    return(XSLT_FUNC_TEXT);
	else if (IS_XSLT_NAME(node, "template"))
	    return(0);
	else if (IS_XSLT_NAME(node, "transform"))
	    return(0);

    } else if (*(node->name) == 'v') {
	if (IS_XSLT_NAME(node, "value-of"))
	    return(XSLT_FUNC_VALUEOF);
	else if (IS_XSLT_NAME(node, "variable"))
	    return(XSLT_FUNC_VARIABLE);

    } else if (*(node->name) == 'w') {
	if (IS_XSLT_NAME(node, "when"))
	    return(XSLT_FUNC_WHEN);
	if (IS_XSLT_NAME(node, "with-param"))
	    return(XSLT_FUNC_WITHPARAM);
    }
    return(0);
}
