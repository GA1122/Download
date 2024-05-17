 PHP_FUNCTION(mdecrypt_generic)
 {
 	zval *mcryptind;
 	char *data;
 	int data_len;
  	php_mcrypt *pm;
  	char* data_s;
  	int block_size, data_size;
// 
  	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &mcryptind, &data, &data_len) == FAILURE) {
  		return;
  	}
// 
  	ZEND_FETCH_RESOURCE(pm, php_mcrypt * , &mcryptind, -1, "MCrypt", le_mcrypt);
  	PHP_MCRYPT_INIT_CHECK
  
 	if (data_len == 0) {
 		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An empty string was passed");
 		RETURN_FALSE
 	}
 
 	 
  	if (mcrypt_enc_is_block_mode(pm->td) == 1) {  
  		block_size = mcrypt_enc_get_block_size(pm->td);
  		data_size = (((data_len - 1) / block_size) + 1) * block_size;
// 		if (data_size <= 0) {
// 			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Integer overflow in data size");
// 			RETURN_FALSE;
// 		}
  		data_s = emalloc(data_size + 1);
  		memset(data_s, 0, data_size);
  		memcpy(data_s, data, data_len);
 	} else {  
 		data_size = data_len;
 		data_s = emalloc(data_size + 1);
  		memset(data_s, 0, data_size);
  		memcpy(data_s, data, data_len);
  	}
// 
  	mdecrypt_generic(pm->td, data_s, data_size);
  
  	RETVAL_STRINGL(data_s, data_size, 1);
 	efree(data_s);
 }