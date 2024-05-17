static void destroy_phar_data_only(zval *zv)  
{
	phar_archive_data *phar_data = (phar_archive_data *) Z_PTR_P(zv);

	if (EG(exception) || --phar_data->refcount < 0) {
		phar_destroy_phar_data(phar_data);
	}
}
 
