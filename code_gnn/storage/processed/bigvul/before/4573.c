static X509 * php_openssl_x509_from_zval(zval * val, int makeresource, zend_resource **resourceval)
{
	X509 *cert = NULL;

	if (resourceval) {
		*resourceval = NULL;
	}
	if (Z_TYPE_P(val) == IS_RESOURCE) {
		 
		void * what;
		zend_resource *res = Z_RES_P(val);

		what = zend_fetch_resource(res, "OpenSSL X.509", le_x509);
		if (!what) {
			return NULL;
		}
		 
		if (resourceval) {
			*resourceval = res;
			Z_ADDREF_P(val);
		}
		return (X509*)what;
	}

	if (!(Z_TYPE_P(val) == IS_STRING || Z_TYPE_P(val) == IS_OBJECT)) {
		return NULL;
	}

	 
	convert_to_string_ex(val);

	if (Z_STRLEN_P(val) > 7 && memcmp(Z_STRVAL_P(val), "file://", sizeof("file://") - 1) == 0) {
		 
		BIO *in;

		if (php_openssl_open_base_dir_chk(Z_STRVAL_P(val) + (sizeof("file://") - 1))) {
			return NULL;
		}

		in = BIO_new_file(Z_STRVAL_P(val) + (sizeof("file://") - 1), "r");
		if (in == NULL) {
			return NULL;
		}
		cert = PEM_read_bio_X509(in, NULL, NULL, NULL);
		BIO_free(in);
	} else {
		BIO *in;

		in = BIO_new_mem_buf(Z_STRVAL_P(val), (int)Z_STRLEN_P(val));
		if (in == NULL) {
			return NULL;
		}
#ifdef TYPEDEF_D2I_OF
		cert = (X509 *) PEM_ASN1_read_bio((d2i_of_void *)d2i_X509, PEM_STRING_X509, in, NULL, NULL, NULL);
#else
		cert = (X509 *) PEM_ASN1_read_bio((char *(*)())d2i_X509, PEM_STRING_X509, in, NULL, NULL, NULL);
#endif
		BIO_free(in);
	}

	if (cert && makeresource && resourceval) {
		*resourceval = zend_register_resource(cert, le_x509);
	}
	return cert;
}