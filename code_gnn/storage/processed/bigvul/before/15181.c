PHP_MSHUTDOWN_FUNCTION(gd)
{
#if HAVE_LIBT1
	T1_CloseLib();
#endif
#if HAVE_GD_FONTMUTEX && HAVE_LIBFREETYPE
	gdFontCacheMutexShutdown();
#endif
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
