static void xmlwriter_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
	xmlwriter_object *intern;

	intern = (xmlwriter_object *) rsrc->ptr;
	xmlwriter_free_resource_ptr(intern TSRMLS_CC);
}
