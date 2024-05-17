PHPAPI void php_pcre_split_impl(pcre_cache_entry *pce, char *subject, int subject_len, zval *return_value,
	long limit_val, long flags TSRMLS_DC)
{
	pcre_extra		*extra = NULL;		 
	pcre			*re_bump = NULL;	 
	pcre_extra		*extra_bump = NULL;	 
	pcre_extra		 extra_data;		 
	int				*offsets;			 
	int				 size_offsets;		 
	int				 exoptions = 0;		 
	int				 count = 0;			 
	int				 start_offset;		 
	int				 next_offset;		 
	int				 g_notempty = 0;	 
	char			*last_match;		 
	int				 rc;
	int				 no_empty;			 
	int				 delim_capture; 	 
	int				 offset_capture;	 

	no_empty = flags & PREG_SPLIT_NO_EMPTY;
	delim_capture = flags & PREG_SPLIT_DELIM_CAPTURE;
	offset_capture = flags & PREG_SPLIT_OFFSET_CAPTURE;
	
	if (limit_val == 0) {
		limit_val = -1;
	}

	if (extra == NULL) {
		extra_data.flags = PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
		extra = &extra_data;
	}
	extra->match_limit = PCRE_G(backtrack_limit);
	extra->match_limit_recursion = PCRE_G(recursion_limit);
	
	 
	array_init(return_value);

	 
	rc = pcre_fullinfo(pce->re, extra, PCRE_INFO_CAPTURECOUNT, &size_offsets);
	if (rc < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Internal pcre_fullinfo() error %d", rc);
		RETURN_FALSE;
	}
	size_offsets = (size_offsets + 1) * 3;
	offsets = (int *)safe_emalloc(size_offsets, sizeof(int), 0);
	
	 
	start_offset = 0;
	next_offset = 0;
	last_match = subject;
	PCRE_G(error_code) = PHP_PCRE_NO_ERROR;
	
	 
	while ((limit_val == -1 || limit_val > 1)) {
		count = pcre_exec(pce->re, extra, subject,
						  subject_len, start_offset,
						  exoptions|g_notempty, offsets, size_offsets);

		 
		exoptions |= PCRE_NO_UTF8_CHECK;

		 
		if (count == 0) {
			php_error_docref(NULL TSRMLS_CC,E_NOTICE, "Matched, but too many substrings");
			count = size_offsets/3;
		}
				
		 
		if (count > 0) {
			if (!no_empty || &subject[offsets[0]] != last_match) {

				if (offset_capture) {
					 
					add_offset_pair(return_value, last_match, &subject[offsets[0]]-last_match, next_offset, NULL);
				} else {
					 
					add_next_index_stringl(return_value, last_match,
								   	   &subject[offsets[0]]-last_match, 1);
				}

				 
				if (limit_val != -1)
					limit_val--;
			}
			
			last_match = &subject[offsets[1]];
			next_offset = offsets[1];

			if (delim_capture) {
				int i, match_len;
				for (i = 1; i < count; i++) {
					match_len = offsets[(i<<1)+1] - offsets[i<<1];
					 
					if (!no_empty || match_len > 0) {
						if (offset_capture) {
							add_offset_pair(return_value, &subject[offsets[i<<1]], match_len, offsets[i<<1], NULL);
						} else {
							add_next_index_stringl(return_value,
												   &subject[offsets[i<<1]],
												   match_len, 1);
						}
					}
				}
			}
		} else if (count == PCRE_ERROR_NOMATCH) {
			 
			if (g_notempty != 0 && start_offset < subject_len) {
				if (pce->compile_options & PCRE_UTF8) {
					if (re_bump == NULL) {
						int dummy;

						if ((re_bump = pcre_get_compiled_regex("/./us", &extra_bump, &dummy TSRMLS_CC)) == NULL) {
							RETURN_FALSE;
						}
					}
					count = pcre_exec(re_bump, extra_bump, subject,
							  subject_len, start_offset,
							  exoptions, offsets, size_offsets);
					if (count < 1) {
						php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown error");
						RETURN_FALSE;
					}
				} else {
					offsets[0] = start_offset;
					offsets[1] = start_offset + 1;
				}
			} else
				break;
		} else {
			pcre_handle_exec_error(count TSRMLS_CC);
			break;
		}

		 
		g_notempty = (offsets[1] == offsets[0])? PCRE_NOTEMPTY | PCRE_ANCHORED : 0;
		
		 
		start_offset = offsets[1];
	}


	start_offset = last_match - subject;  

	if (!no_empty || start_offset < subject_len)
	{
		if (offset_capture) {
			 
			add_offset_pair(return_value, &subject[start_offset], subject_len - start_offset, start_offset, NULL);
		} else {
			 
			add_next_index_stringl(return_value, last_match, subject + subject_len - last_match, 1);
		}
	}

	
	 
	efree(offsets);
}
