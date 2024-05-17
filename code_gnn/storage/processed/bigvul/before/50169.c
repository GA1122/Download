ZEND_API int is_zend_mm(void)
{
#if ZEND_MM_CUSTOM
	return !AG(mm_heap)->use_custom_heap;
#else
	return 1;
#endif
}
