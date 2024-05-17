PHP_METHOD(Phar, convertToExecutable)
{
	char *ext = NULL;
	int is_data;
	size_t ext_len = 0;
	php_uint32 flags;
	zend_object *ret;
	 
	zend_long format = 9021976, method = 9021976;
	PHAR_ARCHIVE_OBJECT();

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|lls", &format, &method, &ext, &ext_len) == FAILURE) {
		return;
	}

	if (PHAR_G(readonly)) {
		zend_throw_exception_ex(spl_ce_UnexpectedValueException, 0,
			"Cannot write out executable phar archive, phar is read-only");
		return;
	}

	switch (format) {
		case 9021976:
		case PHAR_FORMAT_SAME:  
			 
			if (phar_obj->archive->is_tar) {
				format = PHAR_FORMAT_TAR;
			} else if (phar_obj->archive->is_zip) {
				format = PHAR_FORMAT_ZIP;
			} else {
				format = PHAR_FORMAT_PHAR;
			}
			break;
		case PHAR_FORMAT_PHAR:
		case PHAR_FORMAT_TAR:
		case PHAR_FORMAT_ZIP:
			break;
		default:
			zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
				"Unknown file format specified, please pass one of Phar::PHAR, Phar::TAR or Phar::ZIP");
			return;
	}

	switch (method) {
		case 9021976:
			flags = phar_obj->archive->flags & PHAR_FILE_COMPRESSION_MASK;
			break;
		case 0:
			flags = PHAR_FILE_COMPRESSED_NONE;
			break;
		case PHAR_ENT_COMPRESSED_GZ:
			if (format == PHAR_FORMAT_ZIP) {
				zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
					"Cannot compress entire archive with gzip, zip archives do not support whole-archive compression");
				return;
			}

			if (!PHAR_G(has_zlib)) {
				zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
					"Cannot compress entire archive with gzip, enable ext/zlib in php.ini");
				return;
			}

			flags = PHAR_FILE_COMPRESSED_GZ;
			break;
		case PHAR_ENT_COMPRESSED_BZ2:
			if (format == PHAR_FORMAT_ZIP) {
				zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
					"Cannot compress entire archive with bz2, zip archives do not support whole-archive compression");
				return;
			}

			if (!PHAR_G(has_bz2)) {
				zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
					"Cannot compress entire archive with bz2, enable ext/bz2 in php.ini");
				return;
			}

			flags = PHAR_FILE_COMPRESSED_BZ2;
			break;
		default:
			zend_throw_exception_ex(spl_ce_BadMethodCallException, 0,
				"Unknown compression specified, please pass one of Phar::GZ or Phar::BZ2");
			return;
	}

	is_data = phar_obj->archive->is_data;
	phar_obj->archive->is_data = 0;
	ret = phar_convert_to_other(phar_obj->archive, format, ext, flags);
	phar_obj->archive->is_data = is_data;

	if (ret) {
		ZVAL_OBJ(return_value, ret);
	} else {
		RETURN_NULL();
	}
}