 PHP_METHOD(Phar, offsetExists)
 {
 	char *fname;
 	size_t fname_len;
 	phar_entry_info *entry;
  
         PHAR_ARCHIVE_OBJECT();
  
       if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &fname, &fname_len) == FAILURE) {
//        if (zend_parse_parameters(ZEND_NUM_ARGS(), "p", &fname, &fname_len) == FAILURE) {
                 return;
         }
  
 	if (zend_hash_str_exists(&phar_obj->archive->manifest, fname, (uint) fname_len)) {
 		if (NULL != (entry = zend_hash_str_find_ptr(&phar_obj->archive->manifest, fname, (uint) fname_len))) {
 			if (entry->is_deleted) {
 				 
 				RETURN_FALSE;
 			}
 		}
 
 		if (fname_len >= sizeof(".phar")-1 && !memcmp(fname, ".phar", sizeof(".phar")-1)) {
 			 
 			RETURN_FALSE;
 		}
 		RETURN_TRUE;
 	} else {
 		if (zend_hash_str_exists(&phar_obj->archive->virtual_dirs, fname, (uint) fname_len)) {
 			RETURN_TRUE;
 		}
 		RETURN_FALSE;
 	}
 }