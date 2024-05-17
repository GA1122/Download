static PHP_INI_MH(OnUpdate_zlib_output_compression)
{
	int status, int_value;
	char *ini_value;

	if (new_value == NULL) {
		return FAILURE;
	}

	if (!strncasecmp(new_value, "off", sizeof("off"))) {
		new_value = "0";
		new_value_length = sizeof("0");
	} else if (!strncasecmp(new_value, "on", sizeof("on"))) {
		new_value = "1";
		new_value_length = sizeof("1");
	}

	int_value = zend_atoi(new_value, new_value_length);
	ini_value = zend_ini_string("output_handler", sizeof("output_handler"), 0);

	if (ini_value && *ini_value && int_value) {
		php_error_docref("ref.outcontrol" TSRMLS_CC, E_CORE_ERROR, "Cannot use both zlib.output_compression and output_handler together!!");
		return FAILURE;
	}
	if (stage == PHP_INI_STAGE_RUNTIME) {
		status = php_output_get_status(TSRMLS_C);
		if (status & PHP_OUTPUT_SENT) {
			php_error_docref("ref.outcontrol" TSRMLS_CC, E_WARNING, "Cannot change zlib.output_compression - headers already sent");
			return FAILURE;
		}
	}

	status = OnUpdateLong(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);

	ZLIBG(output_compression) = ZLIBG(output_compression_default);
	if (stage == PHP_INI_STAGE_RUNTIME && int_value) {
		if (!php_output_handler_started(ZEND_STRL(PHP_ZLIB_OUTPUT_HANDLER_NAME) TSRMLS_CC)) {
			php_zlib_output_compression_start(TSRMLS_C);
		}
	}

	return status;
}
