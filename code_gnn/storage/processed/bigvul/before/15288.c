static int stream_array_from_fd_set(zval *stream_array, fd_set *fds TSRMLS_DC)
{
	zval **elem, **dest_elem;
	php_stream *stream;
	HashTable *new_hash;
	int ret = 0;

	if (Z_TYPE_P(stream_array) != IS_ARRAY) {
		return 0;
	}
	ALLOC_HASHTABLE(new_hash);
	zend_hash_init(new_hash, zend_hash_num_elements(Z_ARRVAL_P(stream_array)), NULL, ZVAL_PTR_DTOR, 0);

	for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(stream_array));
		 zend_hash_has_more_elements(Z_ARRVAL_P(stream_array)) == SUCCESS;
		 zend_hash_move_forward(Z_ARRVAL_P(stream_array))) {

		int type;
		char *key;
		uint key_len;
		ulong num_ind;
		 
		int tmp_fd;


		type = zend_hash_get_current_key_ex(Z_ARRVAL_P(stream_array),
				&key, &key_len, &num_ind, 0, NULL);
		if (type == HASH_KEY_NON_EXISTANT ||
			zend_hash_get_current_data(Z_ARRVAL_P(stream_array), (void **) &elem) == FAILURE) {
			continue;  
		}

		php_stream_from_zval_no_verify(stream, elem);
		if (stream == NULL) {
			continue;
		}
		 
		if (SUCCESS == php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL, (void*)&tmp_fd, 1) && tmp_fd != -1) {

			php_socket_t this_fd = (php_socket_t)tmp_fd;

			if (PHP_SAFE_FD_ISSET(this_fd, fds)) {
				if (type == HASH_KEY_IS_LONG) {
					zend_hash_index_update(new_hash, num_ind, (void *)elem, sizeof(zval *), (void **)&dest_elem);
				} else {  
					zend_hash_update(new_hash, key, key_len, (void *)elem, sizeof(zval *), (void **)&dest_elem);
				}
				
				if (dest_elem) {
					zval_add_ref(dest_elem);
				}
				ret++;
				continue;
			}
		}
	}

	 
	zend_hash_destroy(Z_ARRVAL_P(stream_array));
	efree(Z_ARRVAL_P(stream_array));

	zend_hash_internal_pointer_reset(new_hash);
	Z_ARRVAL_P(stream_array) = new_hash;

	return ret;
}
