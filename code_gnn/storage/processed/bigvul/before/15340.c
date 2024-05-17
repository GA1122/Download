static void xmlwriter_object_free_storage(void *object TSRMLS_DC)
{
	ze_xmlwriter_object * intern = (ze_xmlwriter_object *) object;
	if (!intern) {
		return;
	}
	if (intern->xmlwriter_ptr) {
		xmlwriter_free_resource_ptr(intern->xmlwriter_ptr TSRMLS_CC);
	}
	intern->xmlwriter_ptr = NULL;
	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	
	efree(intern);
}
