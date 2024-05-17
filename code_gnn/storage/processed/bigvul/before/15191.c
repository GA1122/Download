static void php_free_gd_image(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	gdImageDestroy((gdImagePtr) rsrc->ptr);
}
