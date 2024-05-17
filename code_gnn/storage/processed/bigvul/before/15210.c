static void php_imagettftext_common(INTERNAL_FUNCTION_PARAMETERS, int mode, int extended)
{
	zval *IM, *EXT = NULL;
	gdImagePtr im=NULL;
	long col = -1, x = -1, y = -1;
	int str_len, fontname_len, i, brect[8];
	double ptsize, angle;
	char *str = NULL, *fontname = NULL;
	char *error = NULL;
	int argc = ZEND_NUM_ARGS();
#if HAVE_GD_STRINGFTEX
	gdFTStringExtra strex = {0};
#endif

#if !HAVE_GD_STRINGFTEX
	assert(!extended);
#endif

	if (mode == TTFTEXT_BBOX) {
		if (argc < 4 || argc > ((extended) ? 5 : 4)) {
			ZEND_WRONG_PARAM_COUNT();
		} else if (zend_parse_parameters(argc TSRMLS_CC, "ddss|a", &ptsize, &angle, &fontname, &fontname_len, &str, &str_len, &EXT) == FAILURE) {
			RETURN_FALSE;
		}
	} else {
		if (argc < 8 || argc > ((extended) ? 9 : 8)) {
			ZEND_WRONG_PARAM_COUNT();
		} else if (zend_parse_parameters(argc TSRMLS_CC, "rddlllss|a", &IM, &ptsize, &angle, &x, &y, &col, &fontname, &fontname_len, &str, &str_len, &EXT) == FAILURE) {
			RETURN_FALSE;
		}
		ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	}

	 
	angle = angle * (M_PI/180);

#if HAVE_GD_STRINGFTEX
	if (extended && EXT) {	 
		HashPosition pos;

		 
		zend_hash_internal_pointer_reset_ex(HASH_OF(EXT), &pos);
		do {
			zval ** item;
			char * key;
			ulong num_key;

			if (zend_hash_get_current_key_ex(HASH_OF(EXT), &key, NULL, &num_key, 0, &pos) != HASH_KEY_IS_STRING) {
				continue;
			}

			if (zend_hash_get_current_data_ex(HASH_OF(EXT), (void **) &item, &pos) == FAILURE) {
				continue;
			}
		
			if (strcmp("linespacing", key) == 0) {
				convert_to_double_ex(item);
				strex.flags |= gdFTEX_LINESPACE;
				strex.linespacing = Z_DVAL_PP(item);
			}

		} while (zend_hash_move_forward_ex(HASH_OF(EXT), &pos) == SUCCESS);
	}
#endif

#ifdef VIRTUAL_DIR
	{
		char tmp_font_path[MAXPATHLEN];

		if (!VCWD_REALPATH(fontname, tmp_font_path)) {
			fontname = NULL;
		}
	}
#endif

	PHP_GD_CHECK_OPEN_BASEDIR(fontname, "Invalid font filename");
	
#ifdef USE_GD_IMGSTRTTF
# if HAVE_GD_STRINGFTEX
	if (extended) {
		error = gdImageStringFTEx(im, brect, col, fontname, ptsize, angle, x, y, str, &strex);
	}
	else
# endif

# if HAVE_GD_STRINGFT
	error = gdImageStringFT(im, brect, col, fontname, ptsize, angle, x, y, str);
# elif HAVE_GD_STRINGTTF
	error = gdImageStringTTF(im, brect, col, fontname, ptsize, angle, x, y, str);
# endif

#endif

	if (error) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", error);
		RETURN_FALSE;
	}

	array_init(return_value);

	 
	for (i = 0; i < 8; i++) {
		add_next_index_long(return_value, brect[i]);
	}
}