PHP_FUNCTION(imagecreatefromstring)
{
	zval **data;
	gdImagePtr im;
	int imtype;
	char sig[8];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Z", &data) == FAILURE) {
		return;
	}

	convert_to_string_ex(data);
	if (Z_STRLEN_PP(data) < 8) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty string or invalid image");
		RETURN_FALSE;
	}

	memcpy(sig, Z_STRVAL_PP(data), 8);

	imtype = _php_image_type(sig);

	switch (imtype) {
		case PHP_GDIMG_TYPE_JPG:
#ifdef HAVE_GD_JPG
			im = _php_image_create_from_string(data, "JPEG", gdImageCreateFromJpegCtx TSRMLS_CC);
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "No JPEG support in this PHP build");
			RETURN_FALSE;
#endif
			break;

		case PHP_GDIMG_TYPE_PNG:
#ifdef HAVE_GD_PNG
			im = _php_image_create_from_string(data, "PNG", gdImageCreateFromPngCtx TSRMLS_CC);
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "No PNG support in this PHP build");
			RETURN_FALSE;
#endif
			break;

		case PHP_GDIMG_TYPE_GIF:
#ifdef HAVE_GD_GIF_READ
			im = _php_image_create_from_string(data, "GIF", gdImageCreateFromGifCtx TSRMLS_CC);
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "No GIF support in this PHP build");
			RETURN_FALSE;
#endif
			break;

		case PHP_GDIMG_TYPE_WBM:
#ifdef HAVE_GD_WBMP
			im = _php_image_create_from_string(data, "WBMP", gdImageCreateFromWBMPCtx TSRMLS_CC);
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "No WBMP support in this PHP build");
			RETURN_FALSE;
#endif
			break;

		case PHP_GDIMG_TYPE_GD2:
#ifdef HAVE_GD_GD2
			im = _php_image_create_from_string(data, "GD2", gdImageCreateFromGd2Ctx TSRMLS_CC);
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "No GD2 support in this PHP build");
			RETURN_FALSE;
#endif
			break;

		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Data is not in a recognized format");
			RETURN_FALSE;
	}

	if (!im) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Couldn't create GD Image Stream out of Data");
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE(return_value, im, le_gd);
}
