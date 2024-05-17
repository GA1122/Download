 PHP_FUNCTION(xml_set_element_handler)
 {
	xml_parser *parser;
	zval *pind, **shdl, **ehdl;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rZZ", &pind, &shdl, &ehdl) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(parser,xml_parser *, &pind, -1, "XML Parser", le_xml_parser);

	xml_set_handler(&parser->startElementHandler, shdl);
	xml_set_handler(&parser->endElementHandler, ehdl);
	XML_SetElementHandler(parser->parser, _xml_startElementHandler, _xml_endElementHandler);
	RETVAL_TRUE;
 }
