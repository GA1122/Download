static PHP_FUNCTION(xmlwriter_start_element)
{
	php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAM_PASSTHRU, xmlTextWriterStartElement, "Invalid Element Name");
}
