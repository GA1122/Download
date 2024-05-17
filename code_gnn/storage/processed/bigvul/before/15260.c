PHP_FUNCTION(stream_get_transports)
{
	HashTable *stream_xport_hash;
	char *stream_xport;
	uint stream_xport_len;
	ulong num_key;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((stream_xport_hash = php_stream_xport_get_hash())) {
		HashPosition pos;
		array_init(return_value);
		zend_hash_internal_pointer_reset_ex(stream_xport_hash, &pos);
		while (zend_hash_get_current_key_ex(stream_xport_hash,
					&stream_xport, &stream_xport_len,
					&num_key, 0, &pos) == HASH_KEY_IS_STRING) {
			add_next_index_stringl(return_value, stream_xport, stream_xport_len - 1, 1);
			zend_hash_move_forward_ex(stream_xport_hash, &pos);
		}
	} else {
		RETURN_FALSE;
	}
}
