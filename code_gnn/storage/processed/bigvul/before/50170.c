static zend_always_inline size_t safe_address(size_t nmemb, size_t size, size_t offset)
{
	int overflow;
	size_t ret = zend_safe_address(nmemb, size, offset, &overflow);

	if (UNEXPECTED(overflow)) {
		zend_error_noreturn(E_ERROR, "Possible integer overflow in memory allocation (%zu * %zu + %zu)", nmemb, size, offset);
		return 0;
	}
	return ret;
}
