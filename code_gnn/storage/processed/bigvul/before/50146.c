ZEND_API void * __zend_malloc(size_t len)
{
	void *tmp = malloc(len);
	if (EXPECTED(tmp)) {
		return tmp;
	}
	zend_out_of_memory();
}
