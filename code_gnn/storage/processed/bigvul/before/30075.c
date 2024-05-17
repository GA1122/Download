static size_t encoding_filter_script_to_internal(unsigned char **to, size_t *to_length, const unsigned char *from, size_t from_length TSRMLS_DC)
{
	const zend_encoding *internal_encoding = zend_multibyte_get_internal_encoding(TSRMLS_C);
	assert(internal_encoding && zend_multibyte_check_lexer_compatibility(internal_encoding));
	return zend_multibyte_encoding_converter(to, to_length, from, from_length, internal_encoding, LANG_SCNG(script_encoding) TSRMLS_CC);
}
