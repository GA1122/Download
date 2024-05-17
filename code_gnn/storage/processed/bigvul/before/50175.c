ZEND_API size_t zend_memory_usage(int real_usage)
{
#if ZEND_MM_STAT
	if (real_usage) {
		return AG(mm_heap)->real_size;
	} else {
		size_t usage = AG(mm_heap)->size;
		return usage;
	}
#endif
	return 0;
}
