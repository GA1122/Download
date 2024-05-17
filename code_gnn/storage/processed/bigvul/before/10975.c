 PHP_FUNCTION(xml_set_notation_decl_handler)
 {
	xml_parser *parser;
	zval *pind, **hdl;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rZ", &pind, &hdl) == FAILURE) {
		return;
	}
	ZEND_FETCH_RESOURCE(parser,xml_parser *, &pind, -1, "XML Parser", le_xml_parser);

	xml_set_handler(&parser->notationDeclHandler, hdl);
	XML_SetNotationDeclHandler(parser->parser, _xml_notationDeclHandler);
	RETVAL_TRUE;
 }
