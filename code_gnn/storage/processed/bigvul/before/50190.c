ZEND_END_MODULE_GLOBALS(exif)

ZEND_DECLARE_MODULE_GLOBALS(exif)

#ifdef ZTS
#define EXIF_G(v) TSRMG(exif_globals_id, zend_exif_globals *, v)
#else
#define EXIF_G(v) (exif_globals.v)
#endif

 

ZEND_INI_MH(OnUpdateEncode)
{
	if (new_value && new_value_length) {
		const zend_encoding **return_list;
		size_t return_size;
		if (FAILURE == zend_multibyte_parse_encoding_list(new_value, new_value_length,
	&return_list, &return_size, 0 TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Illegal encoding ignored: '%s'", new_value);
			return FAILURE;
		}
		efree(return_list);
	}
	return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
}

ZEND_INI_MH(OnUpdateDecode)
{
	if (new_value) {
		const zend_encoding **return_list;
		size_t return_size;
		if (FAILURE == zend_multibyte_parse_encoding_list(new_value, new_value_length,
	&return_list, &return_size, 0 TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Illegal encoding ignored: '%s'", new_value);
			return FAILURE;
		}
		efree(return_list);
	}
	return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
}

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("exif.encode_unicode",          "ISO-8859-15", PHP_INI_ALL, OnUpdateEncode, encode_unicode,    zend_exif_globals, exif_globals)
    STD_PHP_INI_ENTRY("exif.decode_unicode_motorola", "UCS-2BE",     PHP_INI_ALL, OnUpdateDecode, decode_unicode_be, zend_exif_globals, exif_globals)
    STD_PHP_INI_ENTRY("exif.decode_unicode_intel",    "UCS-2LE",     PHP_INI_ALL, OnUpdateDecode, decode_unicode_le, zend_exif_globals, exif_globals)
    STD_PHP_INI_ENTRY("exif.encode_jis",              "",            PHP_INI_ALL, OnUpdateEncode, encode_jis,        zend_exif_globals, exif_globals)
    STD_PHP_INI_ENTRY("exif.decode_jis_motorola",     "JIS",         PHP_INI_ALL, OnUpdateDecode, decode_jis_be,     zend_exif_globals, exif_globals)
    STD_PHP_INI_ENTRY("exif.decode_jis_intel",        "JIS",         PHP_INI_ALL, OnUpdateDecode, decode_jis_le,     zend_exif_globals, exif_globals)
PHP_INI_END()
 
