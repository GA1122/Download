static ZIPARCHIVE_METHOD(addFromString)
{
	struct zip *intern;
	zval *self = getThis();
	zend_string *buffer;
	char *name;
	size_t name_len;
	ze_zip_object *ze_obj;
	struct zip_source *zs;
	int pos = 0;
	int cur_idx;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sS",
			&name, &name_len, &buffer) == FAILURE) {
		return;
	}

	ze_obj = Z_ZIP_P(self);
	if (ze_obj->buffers_cnt) {
		ze_obj->buffers = (char **)erealloc(ze_obj->buffers, sizeof(char *) * (ze_obj->buffers_cnt+1));
		pos = ze_obj->buffers_cnt++;
	} else {
		ze_obj->buffers = (char **)emalloc(sizeof(char *));
		ze_obj->buffers_cnt++;
		pos = 0;
	}
	ze_obj->buffers[pos] = (char *)emalloc(ZSTR_LEN(buffer) + 1);
	memcpy(ze_obj->buffers[pos], ZSTR_VAL(buffer), ZSTR_LEN(buffer) + 1);

	zs = zip_source_buffer(intern, ze_obj->buffers[pos], ZSTR_LEN(buffer), 0);

	if (zs == NULL) {
		RETURN_FALSE;
	}

	cur_idx = zip_name_locate(intern, (const char *)name, 0);
	 
	if (cur_idx >= 0) {
		if (zip_delete(intern, cur_idx) == -1) {
			zip_source_free(zs);
			RETURN_FALSE;
		}
	}

	if (zip_add(intern, name, zs) == -1) {
		zip_source_free(zs);
		RETURN_FALSE;
	} else {
		zip_error_clear(intern);
		RETURN_TRUE;
	}
}
