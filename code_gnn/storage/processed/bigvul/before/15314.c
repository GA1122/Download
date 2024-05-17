static PHP_FUNCTION(xmlwriter_write_comment)
{
	php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAM_PASSTHRU, xmlTextWriterWriteComment, NULL);
}
