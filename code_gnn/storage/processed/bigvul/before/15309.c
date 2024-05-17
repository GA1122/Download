static PHP_FUNCTION(xmlwriter_write_cdata)
{
	php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAM_PASSTHRU, xmlTextWriterWriteCDATA, NULL);
}
