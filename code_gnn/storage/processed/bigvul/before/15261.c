PHP_FUNCTION(stream_get_wrappers)
{
	HashTable *url_stream_wrappers_hash;
	char *stream_protocol;
	int key_flags;
	uint stream_protocol_len = 0;
	ulong num_key;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((url_stream_wrappers_hash = php_stream_get_url_stream_wrappers_hash())) {
		HashPosition pos;
		array_init(return_value);
		for (zend_hash_internal_pointer_reset_ex(url_stream_wrappers_hash, &pos);
			(key_flags = zend_hash_get_current_key_ex(url_stream_wrappers_hash, &stream_protocol, &stream_protocol_len, &num_key, 0, &pos)) != HASH_KEY_NON_EXISTANT;
			zend_hash_move_forward_ex(url_stream_wrappers_hash, &pos)) {
				if (key_flags == HASH_KEY_IS_STRING) {
					add_next_index_stringl(return_value, stream_protocol, stream_protocol_len - 1, 1);
				}
		}
	} else {
		RETURN_FALSE;
	}

}
