static PHP_FUNCTION(xmlwriter_start_pi)
{
	php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAM_PASSTHRU, xmlTextWriterStartPI, "Invalid PI Target");
}
