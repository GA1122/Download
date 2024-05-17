PHP_FUNCTION(imagetypes)
{
	int ret=0;
#ifdef HAVE_GD_GIF_CREATE
	ret = 1;
#endif
#ifdef HAVE_GD_JPG
	ret |= 2;
#endif
#ifdef HAVE_GD_PNG
	ret |= 4;
#endif
#ifdef HAVE_GD_WBMP
	ret |= 8;
#endif
#if defined(HAVE_GD_XPM) && defined(HAVE_GD_BUNDLED)
	ret |= 16;
#endif

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(ret);
}
