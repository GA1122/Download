PHP_METHOD(DateTimeImmutable, createFromMutable)
{
	zval *datetime_object = NULL;
	php_date_obj *new_obj = NULL;
	php_date_obj *old_obj = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O!", &datetime_object, date_ce_date) == FAILURE) {
		return;
	}

	php_date_instantiate(date_ce_immutable, return_value TSRMLS_CC);
	old_obj = (php_date_obj *) zend_object_store_get_object(datetime_object TSRMLS_CC);
	new_obj = (php_date_obj *) zend_object_store_get_object(return_value TSRMLS_CC);

	new_obj->time = timelib_time_ctor();
	*new_obj->time = *old_obj->time;
	if (old_obj->time->tz_abbr) {
		new_obj->time->tz_abbr = strdup(old_obj->time->tz_abbr);
	}
	if (old_obj->time->tz_info) {
		new_obj->time->tz_info = old_obj->time->tz_info;
	}
}
