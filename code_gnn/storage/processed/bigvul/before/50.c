static inline void add_offset_pair(zval *result, char *str, int len, int offset, char *name)
{
	zval *match_pair;

	ALLOC_ZVAL(match_pair);
	array_init(match_pair);
	INIT_PZVAL(match_pair);

	 
	add_next_index_stringl(match_pair, str, len, 1);
	add_next_index_long(match_pair, offset);
	
	if (name) {
		zval_add_ref(&match_pair);
		zend_hash_update(Z_ARRVAL_P(result), name, strlen(name)+1, &match_pair, sizeof(zval *), NULL);
	}
	zend_hash_next_index_insert(Z_ARRVAL_P(result), &match_pair, sizeof(zval *), NULL);
}
