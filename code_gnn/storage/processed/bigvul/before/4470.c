static int phar_open_from_fp(php_stream* fp, char *fname, int fname_len, char *alias, int alias_len, int options, phar_archive_data** pphar, int is_data, char **error TSRMLS_DC)  
{
	const char token[] = "__HALT_COMPILER();";
	const char zip_magic[] = "PK\x03\x04";
	const char gz_magic[] = "\x1f\x8b\x08";
	const char bz_magic[] = "BZh";
	char *pos, test = '\0';
	const int window_size = 1024;
	char buffer[1024 + sizeof(token)];  
	const long readsize = sizeof(buffer) - sizeof(token);
	const long tokenlen = sizeof(token) - 1;
	long halt_offset;
	size_t got;
	php_uint32 compression = PHAR_FILE_COMPRESSED_NONE;

	if (error) {
		*error = NULL;
	}

	if (-1 == php_stream_rewind(fp)) {
		MAPPHAR_ALLOC_FAIL("cannot rewind phar \"%s\"")
	}

	buffer[sizeof(buffer)-1] = '\0';
	memset(buffer, 32, sizeof(token));
	halt_offset = 0;

	 
	 
	while(!php_stream_eof(fp)) {
		if ((got = php_stream_read(fp, buffer+tokenlen, readsize)) < (size_t) tokenlen) {
			MAPPHAR_ALLOC_FAIL("internal corruption of phar \"%s\" (truncated entry)")
		}

		if (!test) {
			test = '\1';
			pos = buffer+tokenlen;
			if (!memcmp(pos, gz_magic, 3)) {
				char err = 0;
				php_stream_filter *filter;
				php_stream *temp;
				 
				zval filterparams;

				if (!PHAR_G(has_zlib)) {
					MAPPHAR_ALLOC_FAIL("unable to decompress gzipped phar archive \"%s\" to temporary file, enable zlib extension in php.ini")
				}
				array_init(&filterparams);
 
#ifndef MAX_WBITS
#define MAX_WBITS 15
#endif
				add_assoc_long(&filterparams, "window", MAX_WBITS + 32);

				 
				if (!(temp = php_stream_fopen_tmpfile())) {
					MAPPHAR_ALLOC_FAIL("unable to create temporary file for decompression of gzipped phar archive \"%s\"")
				}

				php_stream_rewind(fp);
				filter = php_stream_filter_create("zlib.inflate", &filterparams, php_stream_is_persistent(fp) TSRMLS_CC);

				if (!filter) {
					err = 1;
					add_assoc_long(&filterparams, "window", MAX_WBITS);
					filter = php_stream_filter_create("zlib.inflate", &filterparams, php_stream_is_persistent(fp) TSRMLS_CC);
					zval_dtor(&filterparams);

					if (!filter) {
						php_stream_close(temp);
						MAPPHAR_ALLOC_FAIL("unable to decompress gzipped phar archive \"%s\", ext/zlib is buggy in PHP versions older than 5.2.6")
					}
				} else {
					zval_dtor(&filterparams);
				}

				php_stream_filter_append(&temp->writefilters, filter);

				if (SUCCESS != php_stream_copy_to_stream_ex(fp, temp, PHP_STREAM_COPY_ALL, NULL)) {
					if (err) {
						php_stream_close(temp);
						MAPPHAR_ALLOC_FAIL("unable to decompress gzipped phar archive \"%s\", ext/zlib is buggy in PHP versions older than 5.2.6")
					}
					php_stream_close(temp);
					MAPPHAR_ALLOC_FAIL("unable to decompress gzipped phar archive \"%s\" to temporary file")
				}

				php_stream_filter_flush(filter, 1);
				php_stream_filter_remove(filter, 1 TSRMLS_CC);
				php_stream_close(fp);
				fp = temp;
				php_stream_rewind(fp);
				compression = PHAR_FILE_COMPRESSED_GZ;

				 
				test = '\0';
				continue;
			} else if (!memcmp(pos, bz_magic, 3)) {
				php_stream_filter *filter;
				php_stream *temp;

				if (!PHAR_G(has_bz2)) {
					MAPPHAR_ALLOC_FAIL("unable to decompress bzipped phar archive \"%s\" to temporary file, enable bz2 extension in php.ini")
				}

				 
				if (!(temp = php_stream_fopen_tmpfile())) {
					MAPPHAR_ALLOC_FAIL("unable to create temporary file for decompression of bzipped phar archive \"%s\"")
				}

				php_stream_rewind(fp);
				filter = php_stream_filter_create("bzip2.decompress", NULL, php_stream_is_persistent(fp) TSRMLS_CC);

				if (!filter) {
					php_stream_close(temp);
					MAPPHAR_ALLOC_FAIL("unable to decompress bzipped phar archive \"%s\", filter creation failed")
				}

				php_stream_filter_append(&temp->writefilters, filter);

				if (SUCCESS != php_stream_copy_to_stream_ex(fp, temp, PHP_STREAM_COPY_ALL, NULL)) {
					php_stream_close(temp);
					MAPPHAR_ALLOC_FAIL("unable to decompress bzipped phar archive \"%s\" to temporary file")
				}

				php_stream_filter_flush(filter, 1);
				php_stream_filter_remove(filter, 1 TSRMLS_CC);
				php_stream_close(fp);
				fp = temp;
				php_stream_rewind(fp);
				compression = PHAR_FILE_COMPRESSED_BZ2;

				 
				test = '\0';
				continue;
			}

			if (!memcmp(pos, zip_magic, 4)) {
				php_stream_seek(fp, 0, SEEK_END);
				return phar_parse_zipfile(fp, fname, fname_len, alias, alias_len, pphar, error TSRMLS_CC);
			}

			if (got > 512) {
				if (phar_is_tar(pos, fname)) {
					php_stream_rewind(fp);
					return phar_parse_tarfile(fp, fname, fname_len, alias, alias_len, pphar, is_data, compression, error TSRMLS_CC);
				}
			}
		}

		if (got > 0 && (pos = phar_strnstr(buffer, got + sizeof(token), token, sizeof(token)-1)) != NULL) {
			halt_offset += (pos - buffer);  
			return phar_parse_pharfile(fp, fname, fname_len, alias, alias_len, halt_offset, pphar, compression, error TSRMLS_CC);
		}

		halt_offset += got;
		memmove(buffer, buffer + window_size, tokenlen);  
	}

	MAPPHAR_ALLOC_FAIL("internal corruption of phar \"%s\" (__HALT_COMPILER(); not found)")
}
 