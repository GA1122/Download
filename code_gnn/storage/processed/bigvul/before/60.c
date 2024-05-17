PHPAPI char *php_pcre_replace_impl(pcre_cache_entry *pce, char *subject, int subject_len, zval *replace_val, 
	int is_callable_replace, int *result_len, int limit, int *replace_count TSRMLS_DC)
{
	pcre_extra		*extra = pce->extra; 
	pcre_extra		 extra_data;		 
	int				 exoptions = 0;		 
	int				 count = 0;			 
	int				*offsets;			 
	char 			**subpat_names;		 
	int				 num_subpats;		 
	int				 size_offsets;		 
	int				 new_len;			 
	int				 alloc_len;			 
	int				 eval_result_len=0;	 
	int				 match_len;			 
	int				 backref;			 
	int				 eval;				 
	int				 start_offset;		 
	int				 g_notempty=0;		 
	int				 replace_len=0;		 
	char			*result,			 
					*replace=NULL,		 
					*new_buf,			 
					*walkbuf,			 
					*walk,				 
					*match,				 
					*piece,				 
					*replace_end=NULL,	 
					*eval_result,		 
					 walk_last;			 
	int				 rc;

	if (extra == NULL) {
		extra_data.flags = PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
		extra = &extra_data;
	}
	extra->match_limit = PCRE_G(backtrack_limit);
	extra->match_limit_recursion = PCRE_G(recursion_limit);

	eval = pce->preg_options & PREG_REPLACE_EVAL;
	if (is_callable_replace) {
		if (eval) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Modifier /e cannot be used with replacement callback");
			return NULL;
		}
	} else {
		replace = Z_STRVAL_P(replace_val);
		replace_len = Z_STRLEN_P(replace_val);
		replace_end = replace + replace_len;
	}

	 
	rc = pcre_fullinfo(pce->re, extra, PCRE_INFO_CAPTURECOUNT, &num_subpats);
	if (rc < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Internal pcre_fullinfo() error %d", rc);
		return NULL;
	}
	num_subpats++;
	size_offsets = num_subpats * 3;

	 
	subpat_names = make_subpats_table(num_subpats, pce TSRMLS_CC);
	if (!subpat_names) {
		return NULL;
	}

	offsets = (int *)safe_emalloc(size_offsets, sizeof(int), 0);
	
	alloc_len = 2 * subject_len + 1;
	result = safe_emalloc(alloc_len, sizeof(char), 0);

	 
	match = NULL;
	*result_len = 0;
	start_offset = 0;
	PCRE_G(error_code) = PHP_PCRE_NO_ERROR;
	
	while (1) {
		 
		count = pcre_exec(pce->re, extra, subject, subject_len, start_offset,
						  exoptions|g_notempty, offsets, size_offsets);

		 
		exoptions |= PCRE_NO_UTF8_CHECK;

		 
		if (count == 0) {
			php_error_docref(NULL TSRMLS_CC,E_NOTICE, "Matched, but too many substrings");
			count = size_offsets/3;
		}

		piece = subject + start_offset;

		if (count > 0 && (limit == -1 || limit > 0)) {
			if (replace_count) {
				++*replace_count;
			}
			 
			match = subject + offsets[0];

			new_len = *result_len + offsets[0] - start_offset;  
			
			 
			if (eval) {
				eval_result_len = preg_do_eval(replace, replace_len, subject,
											   offsets, count, &eval_result TSRMLS_CC);
				new_len += eval_result_len;
			} else if (is_callable_replace) {
				 
				eval_result_len = preg_do_repl_func(replace_val, subject, offsets, subpat_names, count, &eval_result TSRMLS_CC);
				new_len += eval_result_len;
			} else {  
				walk = replace;
				walk_last = 0;
				while (walk < replace_end) {
					if ('\\' == *walk || '$' == *walk) {
						if (walk_last == '\\') {
							walk++;
							walk_last = 0;
							continue;
						}
						if (preg_get_backref(&walk, &backref)) {
							if (backref < count)
								new_len += offsets[(backref<<1)+1] - offsets[backref<<1];
							continue;
						}
					}
					new_len++;
					walk++;
					walk_last = walk[-1];
				}
			}

			if (new_len + 1 > alloc_len) {
				alloc_len = 1 + alloc_len + 2 * new_len;
				new_buf = emalloc(alloc_len);
				memcpy(new_buf, result, *result_len);
				efree(result);
				result = new_buf;
			}
			 
			memcpy(&result[*result_len], piece, match-piece);
			*result_len += match-piece;

			 
			walkbuf = result + *result_len;
			
			 
			if (eval || is_callable_replace) {
				memcpy(walkbuf, eval_result, eval_result_len);
				*result_len += eval_result_len;
				STR_FREE(eval_result);
			} else {  
				walk = replace;
				walk_last = 0;
				while (walk < replace_end) {
					if ('\\' == *walk || '$' == *walk) {
						if (walk_last == '\\') {
							*(walkbuf-1) = *walk++;
							walk_last = 0;
							continue;
						}
						if (preg_get_backref(&walk, &backref)) {
							if (backref < count) {
								match_len = offsets[(backref<<1)+1] - offsets[backref<<1];
								memcpy(walkbuf, subject + offsets[backref<<1], match_len);
								walkbuf += match_len;
							}
							continue;
						}
					}
					*walkbuf++ = *walk++;
					walk_last = walk[-1];
				}
				*walkbuf = '\0';
				 
				*result_len += walkbuf - (result + *result_len);
			}

			if (limit != -1)
				limit--;

		} else if (count == PCRE_ERROR_NOMATCH || limit == 0) {
			 
			if (g_notempty != 0 && start_offset < subject_len) {
				offsets[0] = start_offset;
				offsets[1] = start_offset + 1;
				memcpy(&result[*result_len], piece, 1);
				(*result_len)++;
			} else {
				new_len = *result_len + subject_len - start_offset;
				if (new_len + 1 > alloc_len) {
					alloc_len = new_len + 1;  
					new_buf = safe_emalloc(alloc_len, sizeof(char), 0);
					memcpy(new_buf, result, *result_len);
					efree(result);
					result = new_buf;
				}
				 
				memcpy(&result[*result_len], piece, subject_len - start_offset);
				*result_len += subject_len - start_offset;
				result[*result_len] = '\0';
				break;
			}
		} else {
			pcre_handle_exec_error(count TSRMLS_CC);
			efree(result);
			result = NULL;
			break;
		}
			
		 
		g_notempty = (offsets[1] == offsets[0])? PCRE_NOTEMPTY | PCRE_ANCHORED : 0;
		
		 
		start_offset = offsets[1];
	}

	efree(offsets);
	efree(subpat_names);

	return result;
}
