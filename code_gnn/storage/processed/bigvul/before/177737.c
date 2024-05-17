 PHPAPI void php_pcre_match_impl(pcre_cache_entry *pce, char *subject, int subject_len, zval *return_value,
 	zval *subpats, int global, int use_flags, long flags, long start_offset TSRMLS_DC)
 {
 	zval			*result_set,		 
 				   **match_sets = NULL;	 
 	pcre_extra		*extra = pce->extra; 
 	pcre_extra		 extra_data;		 
 	int				 exoptions = 0;		 
 	int				 count = 0;			 
 	int				*offsets;			 
 	int				 num_subpats;		 
 	int				 size_offsets;		 
 	int				 matched;			 
 	int				 g_notempty = 0;	 
 	const char	   **stringlist;		 
 	char 		   **subpat_names;		 
 	int				 i, rc;
 	int				 subpats_order;		 
 	int				 offset_capture;     
 
 	 
 	if (subpats != NULL) {
 		zval_dtor(subpats);
 		array_init(subpats);
 	}
 
 	subpats_order = global ? PREG_PATTERN_ORDER : 0;
 
 	if (use_flags) {
 		offset_capture = flags & PREG_OFFSET_CAPTURE;
 
 		 
 		if (flags & 0xff) {
 			subpats_order = flags & 0xff;
 		}
 		if ((global && (subpats_order < PREG_PATTERN_ORDER || subpats_order > PREG_SET_ORDER)) ||
 			(!global && subpats_order != 0)) {
 			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid flags specified");
 			return;
 		}
 	} else {
 		offset_capture = 0;
 	}
 
 	 
 	if (start_offset < 0) {
 		start_offset = subject_len + start_offset;
 		if (start_offset < 0) {
 			start_offset = 0;
 		}
 	}
 
 	if (extra == NULL) {
 		extra_data.flags = PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
 		extra = &extra_data;
 	}
 	extra->match_limit = PCRE_G(backtrack_limit);
 	extra->match_limit_recursion = PCRE_G(recursion_limit);
 
 	 
 	rc = pcre_fullinfo(pce->re, extra, PCRE_INFO_CAPTURECOUNT, &num_subpats);
 	if (rc < 0) {
 		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Internal pcre_fullinfo() error %d", rc);
 		RETURN_FALSE;
 	}
 	num_subpats++;
 	size_offsets = num_subpats * 3;
 
 	 
 	subpat_names = make_subpats_table(num_subpats, pce TSRMLS_CC);
 	if (!subpat_names) {
 		RETURN_FALSE;
         }
  
         offsets = (int *)safe_emalloc(size_offsets, sizeof(int), 0);
//        memset(offsets, 0, size_offsets*sizeof(int));
          
         if (global && subpats && subpats_order == PREG_PATTERN_ORDER) {
                 match_sets = (zval **)safe_emalloc(num_subpats, sizeof(zval *), 0);
 		for (i=0; i<num_subpats; i++) {
 			ALLOC_ZVAL(match_sets[i]);
 			array_init(match_sets[i]);
 			INIT_PZVAL(match_sets[i]);
 		}
 	}
 
 	matched = 0;
 	PCRE_G(error_code) = PHP_PCRE_NO_ERROR;
 	
 	do {
 		 
 		count = pcre_exec(pce->re, extra, subject, subject_len, start_offset,
 						  exoptions|g_notempty, offsets, size_offsets);
 
 		 
 		exoptions |= PCRE_NO_UTF8_CHECK;
 
 		 
 		if (count == 0) {
 			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Matched, but too many substrings");
 			count = size_offsets/3;
 		}
 
 		 
 		if (count > 0) {
 			matched++;
 
 			 
 			if (subpats != NULL) {
 				 
 				if (pcre_get_substring_list(subject, offsets, count, &stringlist) < 0) {
 					efree(subpat_names);
 					efree(offsets);
 					if (match_sets) efree(match_sets);
 					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Get subpatterns list failed");
 					RETURN_FALSE;
 				}
 
 				if (global) {	 
 					if (subpats && subpats_order == PREG_PATTERN_ORDER) {
 						 
 						for (i = 0; i < count; i++) {
 							if (offset_capture) {
 								add_offset_pair(match_sets[i], (char *)stringlist[i],
 												offsets[(i<<1)+1] - offsets[i<<1], offsets[i<<1], NULL);
 							} else {
 								add_next_index_stringl(match_sets[i], (char *)stringlist[i],
 													   offsets[(i<<1)+1] - offsets[i<<1], 1);
 							}
 						}
 						 
 						if (count < num_subpats) {
 							for (; i < num_subpats; i++) {
 								add_next_index_string(match_sets[i], "", 1);
 							}
 						}
 					} else {
 						 
 						ALLOC_ZVAL(result_set);
 						array_init(result_set);
 						INIT_PZVAL(result_set);
 						
 						 
 						for (i = 0; i < count; i++) {
 							if (offset_capture) {
 								add_offset_pair(result_set, (char *)stringlist[i],
 												offsets[(i<<1)+1] - offsets[i<<1], offsets[i<<1], subpat_names[i]);
 							} else {
 								if (subpat_names[i]) {
 									add_assoc_stringl(result_set, subpat_names[i], (char *)stringlist[i],
 														   offsets[(i<<1)+1] - offsets[i<<1], 1);
 								}
 								add_next_index_stringl(result_set, (char *)stringlist[i],
 													   offsets[(i<<1)+1] - offsets[i<<1], 1);
 							}
 						}
 						 
 						zend_hash_next_index_insert(Z_ARRVAL_P(subpats), &result_set, sizeof(zval *), NULL);
 					}
 				} else {			 
 					 
 					for (i = 0; i < count; i++) {
 						if (offset_capture) {
 							add_offset_pair(subpats, (char *)stringlist[i],
 											offsets[(i<<1)+1] - offsets[i<<1],
 											offsets[i<<1], subpat_names[i]);
 						} else {
 							if (subpat_names[i]) {
 								add_assoc_stringl(subpats, subpat_names[i], (char *)stringlist[i],
 												  offsets[(i<<1)+1] - offsets[i<<1], 1);
 							}
 							add_next_index_stringl(subpats, (char *)stringlist[i],
 												   offsets[(i<<1)+1] - offsets[i<<1], 1);
 						}
 					}
 				}
 
 				pcre_free((void *) stringlist);
 			}
 		} else if (count == PCRE_ERROR_NOMATCH) {
 			 
 			if (g_notempty != 0 && start_offset < subject_len) {
 				offsets[0] = start_offset;
 				offsets[1] = start_offset + 1;
 			} else
 				break;
 		} else {
 			pcre_handle_exec_error(count TSRMLS_CC);
 			break;
 		}
 		
 		 
 		g_notempty = (offsets[1] == offsets[0])? PCRE_NOTEMPTY | PCRE_ANCHORED : 0;
 		
 		 
 		start_offset = offsets[1];
 	} while (global);
 
 	 
 	if (global && subpats && subpats_order == PREG_PATTERN_ORDER) {
 		for (i = 0; i < num_subpats; i++) {
 			if (subpat_names[i]) {
 				zend_hash_update(Z_ARRVAL_P(subpats), subpat_names[i],
 								 strlen(subpat_names[i])+1, &match_sets[i], sizeof(zval *), NULL);
 				Z_ADDREF_P(match_sets[i]);
 			}
 			zend_hash_next_index_insert(Z_ARRVAL_P(subpats), &match_sets[i], sizeof(zval *), NULL);
 		}
 		efree(match_sets);
 	}
 	
 	efree(offsets);
 	efree(subpat_names);
 
 	 
 	if (PCRE_G(error_code) == PHP_PCRE_NO_ERROR) {
 		RETVAL_LONG(matched);
 	} else {
 		RETVAL_FALSE;
 	}
 }