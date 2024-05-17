static int php_zip_status(struct zip *za)  
{
#if LIBZIP_VERSION_MAJOR < 1
	int zep, syp;

	zip_error_get(za, &zep, &syp);
#else
	int zep;
	zip_error_t *err;

	err = zip_get_error(za);
	zep = zip_error_code_zip(err);
	zip_error_fini(err);
#endif
	return zep;
}
 
