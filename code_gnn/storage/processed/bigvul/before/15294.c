static PHP_FUNCTION(xmlwriter_start_attribute)
{
	php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAM_PASSTHRU, xmlTextWriterStartAttribute, "Invalid Attribute Name");
}
