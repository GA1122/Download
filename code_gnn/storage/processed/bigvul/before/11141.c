PHP_METHOD(Phar, isWritable)
{
	php_stream_statbuf ssb;
	PHAR_ARCHIVE_OBJECT();

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!phar_obj->archive->is_writeable) {
		RETURN_FALSE;
	}

	if (SUCCESS != php_stream_stat_path(phar_obj->archive->fname, &ssb)) {
		if (phar_obj->archive->is_brandnew) {
			 
			RETURN_TRUE;
		}
		RETURN_FALSE;
	}

	RETURN_BOOL((ssb.sb.st_mode & (S_IWOTH | S_IWGRP | S_IWUSR)) != 0);
}