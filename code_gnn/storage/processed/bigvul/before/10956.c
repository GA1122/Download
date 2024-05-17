static int exif_process_user_comment(image_info_type *ImageInfo, char **pszInfoPtr, char **pszEncoding, char *szValuePtr, int ByteCount TSRMLS_DC)
{
	int   a;
	char  *decode;
	size_t len;;

	*pszEncoding = NULL;
	 
	if (ByteCount>=8) {
		if (!memcmp(szValuePtr, "UNICODE\0", 8)) {
			*pszEncoding = estrdup((const char*)szValuePtr);
			szValuePtr = szValuePtr+8;
			ByteCount -= 8;
			 
			if (!memcmp(szValuePtr, "\xFE\xFF", 2)) {
				decode = "UCS-2BE";
				szValuePtr = szValuePtr+2;
				ByteCount -= 2;
			} else if (!memcmp(szValuePtr, "\xFF\xFE", 2)) {
				decode = "UCS-2LE";
				szValuePtr = szValuePtr+2;
				ByteCount -= 2;
			} else if (ImageInfo->motorola_intel) {
				decode = ImageInfo->decode_unicode_be;
			} else {
				decode = ImageInfo->decode_unicode_le;
			}
			 
			if (zend_multibyte_encoding_converter(
					(unsigned char**)pszInfoPtr,
					&len,
					(unsigned char*)szValuePtr,
					ByteCount,
					zend_multibyte_fetch_encoding(ImageInfo->encode_unicode TSRMLS_CC),
					zend_multibyte_fetch_encoding(decode TSRMLS_CC)
					TSRMLS_CC) == (size_t)-1) {
				len = exif_process_string_raw(pszInfoPtr, szValuePtr, ByteCount);
			}
			return len;
		} else if (!memcmp(szValuePtr, "ASCII\0\0\0", 8)) {
			*pszEncoding = estrdup((const char*)szValuePtr);
			szValuePtr = szValuePtr+8;
			ByteCount -= 8;
		} else if (!memcmp(szValuePtr, "JIS\0\0\0\0\0", 8)) {
			 
			*pszEncoding = estrdup((const char*)szValuePtr);
			szValuePtr = szValuePtr+8;
			ByteCount -= 8;
			 
			if (zend_multibyte_encoding_converter(
					(unsigned char**)pszInfoPtr,
					&len,
					(unsigned char*)szValuePtr,
					ByteCount,
					zend_multibyte_fetch_encoding(ImageInfo->encode_jis TSRMLS_CC),
					zend_multibyte_fetch_encoding(ImageInfo->motorola_intel ? ImageInfo->decode_jis_be : ImageInfo->decode_jis_le TSRMLS_CC)
					TSRMLS_CC) == (size_t)-1) {
				len = exif_process_string_raw(pszInfoPtr, szValuePtr, ByteCount);
			}
			return len;
		} else if (!memcmp(szValuePtr, "\0\0\0\0\0\0\0\0", 8)) {
			 
			*pszEncoding = estrdup("UNDEFINED");
			szValuePtr = szValuePtr+8;
			ByteCount -= 8;
		}
	}

	 
	if (ByteCount>0) {
		for (a=ByteCount-1;a && szValuePtr[a]==' ';a--) {
			(szValuePtr)[a] = '\0';
		}
	}

	 
	exif_process_string(pszInfoPtr, szValuePtr, ByteCount TSRMLS_CC);
	return strlen(*pszInfoPtr);
}