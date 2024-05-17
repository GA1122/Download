static int stream_array_to_fd_set(zval *stream_array, fd_set *fds, php_socket_t *max_fd TSRMLS_DC)
{
	zval **elem;
	php_stream *stream;
	int cnt = 0;

	if (Z_TYPE_P(stream_array) != IS_ARRAY) {
		return 0;
	}
	for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(stream_array));
		 zend_hash_get_current_data(Z_ARRVAL_P(stream_array), (void **) &elem) == SUCCESS;
		 zend_hash_move_forward(Z_ARRVAL_P(stream_array))) {

		 
		int tmp_fd;

		php_stream_from_zval_no_verify(stream, elem);
		if (stream == NULL) {
			continue;
		}
		 
		if (SUCCESS == php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL, (void*)&tmp_fd, 1) && tmp_fd != -1) {

			php_socket_t this_fd = (php_socket_t)tmp_fd;

			PHP_SAFE_FD_SET(this_fd, fds);

			if (this_fd > *max_fd) {
				*max_fd = this_fd;
			}
			cnt++;
		}
	}
	return cnt ? 1 : 0;
}
