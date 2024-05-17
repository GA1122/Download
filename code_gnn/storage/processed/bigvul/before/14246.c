static xmlParserInputPtr _php_libxml_pre_ext_ent_loader(const char *URL,
		const char *ID, xmlParserCtxtPtr context)
{
	TSRMLS_FETCH();

	 
	if (xmlGenericError == php_libxml_error_handler && PG(modules_activated)) {
		return _php_libxml_external_entity_loader(URL, ID, context);
	} else {
		return _php_libxml_default_entity_loader(URL, ID, context);
	}
}
