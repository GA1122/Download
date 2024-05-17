static ZIPARCHIVE_METHOD(getStatusString)
{
	struct zip *intern;
	zval *self = getThis();
#if LIBZIP_VERSION_MAJOR < 1
	int zep, syp, len;
	char error_string[128];
#else
	zip_error_t *err;
#endif

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

#if LIBZIP_VERSION_MAJOR < 1
	zip_error_get(intern, &zep, &syp);

	len = zip_error_to_str(error_string, 128, zep, syp);
	RETVAL_STRINGL(error_string, len);
#else
	err = zip_get_error(intern);
	RETVAL_STRING(zip_error_strerror(err));
	zip_error_fini(err);
#endif
}
