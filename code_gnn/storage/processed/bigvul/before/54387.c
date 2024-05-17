static ZIPARCHIVE_METHOD(setPassword)
{
	struct zip *intern;
	zval *self = getThis();
	char *password;
	size_t	password_len;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &password, &password_len) == FAILURE) {
		return;
	}

	if (password_len < 1) {
		RETURN_FALSE;
	} else {
		int res = zip_set_default_password(intern, (const char *)password);
		if (res == 0) {
			RETURN_TRUE;
		} else {
			RETURN_FALSE;
		}
	}
}
