static ZEND_MODULE_GLOBALS_CTOR_D(zlib)
{
	zlib_globals->ob_gzhandler = NULL;
    zlib_globals->handler_registered = 0;
}
