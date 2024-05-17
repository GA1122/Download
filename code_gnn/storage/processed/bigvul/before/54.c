PHPAPI pcre_cache_entry* pcre_get_compiled_regex_cache(char *regex, int regex_len TSRMLS_DC)
{
	pcre				*re = NULL;
	pcre_extra			*extra;
	int					 coptions = 0;
	int					 soptions = 0;
	const char			*error;
	int					 erroffset;
	char				 delimiter;
	char				 start_delimiter;
	char				 end_delimiter;
	char				*p, *pp;
	char				*pattern;
	int					 do_study = 0;
	int					 poptions = 0;
	int				count = 0;
	unsigned const char *tables = NULL;
#if HAVE_SETLOCALE
	char				*locale;
#endif
	pcre_cache_entry	*pce;
	pcre_cache_entry	 new_entry;
	char                *tmp = NULL;

#if HAVE_SETLOCALE
# if defined(PHP_WIN32) && defined(ZTS)
	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
# endif
	locale = setlocale(LC_CTYPE, NULL);
#endif

	 
	if (zend_hash_find(&PCRE_G(pcre_cache), regex, regex_len+1, (void **)&pce) == SUCCESS) {
		 
		if (pcre_fullinfo(pce->re, NULL, PCRE_INFO_CAPTURECOUNT, &count) == PCRE_ERROR_BADMAGIC) {
			zend_hash_clean(&PCRE_G(pcre_cache));
		} else {
#if HAVE_SETLOCALE
			if (!strcmp(pce->locale, locale)) {
#endif
				return pce;
#if HAVE_SETLOCALE
			}
#endif
		}
	}
	
	p = regex;
	
	 
	while (isspace((int)*(unsigned char *)p)) p++;
	if (*p == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, 
						 p < regex + regex_len ? "Null byte in regex" : "Empty regular expression");
		return NULL;
	}
	
	 
	delimiter = *p++;
	if (isalnum((int)*(unsigned char *)&delimiter) || delimiter == '\\') {
		php_error_docref(NULL TSRMLS_CC,E_WARNING, "Delimiter must not be alphanumeric or backslash");
		return NULL;
	}

	start_delimiter = delimiter;
	if ((pp = strchr("([{< )]}> )]}>", delimiter)))
