static void _php_image_output(INTERNAL_FUNCTION_PARAMETERS, int image_type, char *tn, void (*func_p)())
{
	zval *imgind;
	char *file = NULL;
	long quality = 0, type = 0;
	gdImagePtr im;
	char *fn = NULL;
	FILE *fp;
	int file_len = 0, argc = ZEND_NUM_ARGS();
	int q = -1, i, t = 1;

	 
	 
	 

	if (zend_parse_parameters(argc TSRMLS_CC, "r|pll", &imgind, &file, &file_len, &quality, &type) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &imgind, -1, "Image", le_gd);

	if (argc > 1) {
		fn = file;
		if (argc == 3) {
			q = quality;
		}
		if (argc == 4) {
			t = type;
		}
	}

	if (argc >= 2 && file_len) {
		PHP_GD_CHECK_OPEN_BASEDIR(fn, "Invalid filename");

		fp = VCWD_FOPEN(fn, "wb");
		if (!fp) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open '%s' for writing", fn);
			RETURN_FALSE;
		}

		switch (image_type) {
			case PHP_GDIMG_CONVERT_WBM:
				if (q == -1) {
					q = 0;
				} else if (q < 0 || q > 255) {
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid threshold value '%d'. It must be between 0 and 255", q);
					q = 0;
				}
				gdImageWBMP(im, q, fp);
				break;
			case PHP_GDIMG_TYPE_JPG:
				(*func_p)(im, fp, q);
				break;
			case PHP_GDIMG_TYPE_WBM:
				for (i = 0; i < gdImageColorsTotal(im); i++) {
					if (gdImageRed(im, i) == 0) break;
				}
				(*func_p)(im, i, fp);
				break;
			case PHP_GDIMG_TYPE_GD:
				if (im->trueColor){
					gdImageTrueColorToPalette(im,1,256);
				}
				(*func_p)(im, fp);
				break;
			case PHP_GDIMG_TYPE_GD2:
				if (q == -1) {
					q = 128;
				}
				(*func_p)(im, fp, q, t);
				break;
			default:
				if (q == -1) {
					q = 128;
				}
				(*func_p)(im, fp, q, t);
				break;
		}
		fflush(fp);
		fclose(fp);
	} else {
		int   b;
		FILE *tmp;
		char  buf[4096];
		char *path;

		tmp = php_open_temporary_file(NULL, NULL, &path TSRMLS_CC);
		if (tmp == NULL) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open temporary file");
			RETURN_FALSE;
		}

		switch (image_type) {
			case PHP_GDIMG_CONVERT_WBM:
 				if (q == -1) {
  					q = 0;
  				} else if (q < 0 || q > 255) {
  					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid threshold value '%d'. It must be between 0 and 255", q);
 					q = 0;
  				}
				gdImageWBMP(im, q, tmp);
				break;
			case PHP_GDIMG_TYPE_JPG:
				(*func_p)(im, tmp, q);
				break;
			case PHP_GDIMG_TYPE_WBM:
				for (i = 0; i < gdImageColorsTotal(im); i++) {
					if (gdImageRed(im, i) == 0) {
						break;
					}
				}
				(*func_p)(im, q, tmp);
				break;
			case PHP_GDIMG_TYPE_GD:
				if (im->trueColor) {
					gdImageTrueColorToPalette(im,1,256);
				}
				(*func_p)(im, tmp);
				break;
			case PHP_GDIMG_TYPE_GD2:
				if (q == -1) {
					q = 128;
				}
				(*func_p)(im, tmp, q, t);
				break;
			default:
				(*func_p)(im, tmp);
				break;
		}

		fseek(tmp, 0, SEEK_SET);

#if APACHE && defined(CHARSET_EBCDIC)
		 

		 
		ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif
		while ((b = fread(buf, 1, sizeof(buf), tmp)) > 0) {
			php_write(buf, b TSRMLS_CC);
		}

		fclose(tmp);
		VCWD_UNLINK((const char *)path);  
		efree(path);
	}
	RETURN_TRUE;
}