static PHP_MINIT_FUNCTION(xmlwriter)
{
#ifdef ZEND_ENGINE_2
	zend_class_entry ce;
#endif

	le_xmlwriter = zend_register_list_destructors_ex(xmlwriter_dtor, NULL, "xmlwriter", module_number);

#ifdef ZEND_ENGINE_2
	memcpy(&xmlwriter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	xmlwriter_object_handlers.clone_obj      = NULL;
	INIT_CLASS_ENTRY(ce, "XMLWriter", xmlwriter_class_functions);
	ce.create_object = xmlwriter_object_new;
	xmlwriter_class_entry_ce = zend_register_internal_class(&ce TSRMLS_CC);
#endif
	return SUCCESS;
}
