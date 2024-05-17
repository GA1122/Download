xmlXPathErr(xmlXPathParserContextPtr ctxt, int error)
{
    if ((error < 0) || (error > MAXERRNO))
	error = MAXERRNO;
    if (ctxt == NULL) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			NULL, NULL, NULL, 0, 0,
			"%s", xmlXPathErrorMessages[error]);
	return;
    }
    ctxt->error = error;
    if (ctxt->context == NULL) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			(const char *) ctxt->base, NULL, NULL,
			ctxt->cur - ctxt->base, 0,
			"%s", xmlXPathErrorMessages[error]);
	return;
    }

     
    xmlResetError(&ctxt->context->lastError);

    ctxt->context->lastError.domain = XML_FROM_XPATH;
    ctxt->context->lastError.code = error + XML_XPATH_EXPRESSION_OK -
                           XPATH_EXPRESSION_OK;
    ctxt->context->lastError.level = XML_ERR_ERROR;
    ctxt->context->lastError.str1 = (char *) xmlStrdup(ctxt->base);
    ctxt->context->lastError.int1 = ctxt->cur - ctxt->base;
    ctxt->context->lastError.node = ctxt->context->debugNode;
    if (ctxt->context->error != NULL) {
	ctxt->context->error(ctxt->context->userData,
	                     &ctxt->context->lastError);
    } else {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, ctxt->context->debugNode, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			(const char *) ctxt->base, NULL, NULL,
			ctxt->cur - ctxt->base, 0,
			"%s", xmlXPathErrorMessages[error]);
    }

}
