static uint32_t zend_always_inline zend_hash_check_size(uint32_t nSize)
{
#if defined(ZEND_WIN32)
	unsigned long index;
#endif

	 
	 
	if (nSize < HT_MIN_SIZE) {
		nSize = HT_MIN_SIZE;
	} else if (UNEXPECTED(nSize >= HT_MAX_SIZE)) {
		zend_error_noreturn(E_ERROR, "Possible integer overflow in memory allocation (%zu * %zu + %zu)", nSize, sizeof(Bucket), sizeof(Bucket));
	}

#if defined(ZEND_WIN32)
	if (BitScanReverse(&index, nSize - 1)) {
		return 0x2 << ((31 - index) ^ 0x1f);
	} else {
		 
		return nSize;
	}
#elif (defined(__GNUC__) || __has_builtin(__builtin_clz))  && defined(PHP_HAVE_BUILTIN_CLZ)
	return 0x2 << (__builtin_clz(nSize - 1) ^ 0x1f);
#else
	nSize -= 1;
	nSize |= (nSize >> 1);
	nSize |= (nSize >> 2);
	nSize |= (nSize >> 4);
	nSize |= (nSize >> 8);
	nSize |= (nSize >> 16);
	return nSize + 1;
#endif
}
