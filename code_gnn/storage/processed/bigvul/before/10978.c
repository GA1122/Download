 PHP_FUNCTION(xml_parse)
 {
	xml_parser *parser;
	zval *pind;
	char *data;
	int data_len, ret;
	long isFinal = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|l", &pind, &data, &data_len, &isFinal) == FAILURE) {
		return;
	}
	ZEND_FETCH_RESOURCE(parser,xml_parser *, &pind, -1, "XML Parser", le_xml_parser);

	parser->isparsing = 1;
	ret = XML_Parse(parser->parser, data, data_len, isFinal);
	parser->isparsing = 0;
	RETVAL_LONG(ret);
}
