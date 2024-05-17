static void php_pgsql_set_default_link(int id TSRMLS_DC)
{
	zend_list_addref(id);

	if (PGG(default_link) != -1) {
		zend_list_delete(PGG(default_link));
	}

	PGG(default_link) = id;
}
