static void php_zip_object_free_storage(zend_object *object)  
{
	ze_zip_object * intern = php_zip_fetch_object(object);
	int i;

	if (!intern) {
		return;
	}
	if (intern->za) {
		if (zip_close(intern->za) != 0) {
			php_error_docref(NULL, E_WARNING, "Cannot destroy the zip context: %s", zip_strerror(intern->za));
			return;
		}
		intern->za = NULL;
	}

	if (intern->buffers_cnt>0) {
		for (i=0; i<intern->buffers_cnt; i++) {
			efree(intern->buffers[i]);
		}
		efree(intern->buffers);
	}

	intern->za = NULL;
	zend_object_std_dtor(&intern->zo);

	if (intern->filename) {
		efree(intern->filename);
	}
}
 
