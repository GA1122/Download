ZEND_API void shutdown_memory_manager(int silent, int full_shutdown)
{
	zend_mm_shutdown(AG(mm_heap), full_shutdown, silent);
}
