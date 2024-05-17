PHP_FUNCTION(stream_select)
{
	zval			*r_array, *w_array, *e_array, **sec = NULL;
	struct timeval	tv;
	struct timeval *tv_p = NULL;
	fd_set			rfds, wfds, efds;
	php_socket_t	max_fd = 0;
	int				retval, sets = 0;
	long			usec = 0;
	int				set_count, max_set_count = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a!a!a!Z!|l", &r_array, &w_array, &e_array, &sec, &usec) == FAILURE)
		return;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	if (r_array != NULL) {
		set_count = stream_array_to_fd_set(r_array, &rfds, &max_fd TSRMLS_CC);
		if (set_count > max_set_count)
			max_set_count = set_count;
		sets += set_count;
	}

	if (w_array != NULL) {
		set_count = stream_array_to_fd_set(w_array, &wfds, &max_fd TSRMLS_CC);
		if (set_count > max_set_count)
			max_set_count = set_count;
		sets += set_count;
	}

	if (e_array != NULL) {
		set_count = stream_array_to_fd_set(e_array, &efds, &max_fd TSRMLS_CC);
		if (set_count > max_set_count)
			max_set_count = set_count;
		sets += set_count;
	}

	if (!sets) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "No stream arrays were passed");
		RETURN_FALSE;
	}

	PHP_SAFE_MAX_FD(max_fd, max_set_count);

	 
	if (sec != NULL) {
		convert_to_long_ex(sec);

		if (Z_LVAL_PP(sec) < 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "The seconds parameter must be greater than 0");
			RETURN_FALSE;
		} else if (usec < 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "The microseconds parameter must be greater than 0");
			RETURN_FALSE;
		}

		 
		if (usec > 999999) {
			tv.tv_sec = Z_LVAL_PP(sec) + (usec / 1000000);
			tv.tv_usec = usec % 1000000;
		} else {
			tv.tv_sec = Z_LVAL_PP(sec);
			tv.tv_usec = usec;
		}

		tv_p = &tv;
	}

	 
	if (r_array != NULL) {

		retval = stream_array_emulate_read_fd_set(r_array TSRMLS_CC);
		if (retval > 0) {
			if (w_array != NULL) {
				zend_hash_clean(Z_ARRVAL_P(w_array));
			}
			if (e_array != NULL) {
				zend_hash_clean(Z_ARRVAL_P(e_array));
			}
			RETURN_LONG(retval);
		}
	}

	retval = php_select(max_fd+1, &rfds, &wfds, &efds, tv_p);

	if (retval == -1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "unable to select [%d]: %s (max_fd=%d)",
				errno, strerror(errno), max_fd);
		RETURN_FALSE;
	}

	if (r_array != NULL) stream_array_from_fd_set(r_array, &rfds TSRMLS_CC);
	if (w_array != NULL) stream_array_from_fd_set(w_array, &wfds TSRMLS_CC);
	if (e_array != NULL) stream_array_from_fd_set(e_array, &efds TSRMLS_CC);

	RETURN_LONG(retval);
}
