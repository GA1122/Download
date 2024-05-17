PHPAPI int php_copy_file_ctx(char *src, char *dest, int src_flg, php_stream_context *ctx TSRMLS_DC)
{
	php_stream *srcstream = NULL, *deststream = NULL;
	int ret = FAILURE;
	php_stream_statbuf src_s, dest_s;

	switch (php_stream_stat_path_ex(src, 0, &src_s, ctx)) {
		case -1:
			 
			goto safe_to_copy;
			break;
		case 0:
			break;
		default:  
			return ret;
	}
	if (S_ISDIR(src_s.sb.st_mode)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The first argument to copy() function cannot be a directory");
		return FAILURE;
	}

	switch (php_stream_stat_path_ex(dest, PHP_STREAM_URL_STAT_QUIET | PHP_STREAM_URL_STAT_NOCACHE, &dest_s, ctx)) {
		case -1:
			 
			goto safe_to_copy;
			break;
		case 0:
			break;
		default:  
			return ret;
	}
	if (S_ISDIR(dest_s.sb.st_mode)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The second argument to copy() function cannot be a directory");
		return FAILURE;
	}
	if (!src_s.sb.st_ino || !dest_s.sb.st_ino) {
		goto no_stat;
	}
	if (src_s.sb.st_ino == dest_s.sb.st_ino && src_s.sb.st_dev == dest_s.sb.st_dev) {
		return ret;
	} else {
		goto safe_to_copy;
	}
no_stat:
	{
		char *sp, *dp;
		int res;

		if ((sp = expand_filepath(src, NULL TSRMLS_CC)) == NULL) {
			return ret;
		}
		if ((dp = expand_filepath(dest, NULL TSRMLS_CC)) == NULL) {
			efree(sp);
			goto safe_to_copy;
		}

		res =
#ifndef PHP_WIN32
			!strcmp(sp, dp);
#else
			!strcasecmp(sp, dp);
#endif

		efree(sp);
		efree(dp);
		if (res) {
			return ret;
		}
	}
safe_to_copy:

	srcstream = php_stream_open_wrapper_ex(src, "rb", src_flg | REPORT_ERRORS, NULL, ctx);

	if (!srcstream) {
		return ret;
	}

	deststream = php_stream_open_wrapper_ex(dest, "wb", REPORT_ERRORS, NULL, ctx);

	if (srcstream && deststream) {
		ret = php_stream_copy_to_stream_ex(srcstream, deststream, PHP_STREAM_COPY_ALL, NULL);
	}
	if (srcstream) {
		php_stream_close(srcstream);
	}
	if (deststream) {
		php_stream_close(deststream);
	}
	return ret;
}
