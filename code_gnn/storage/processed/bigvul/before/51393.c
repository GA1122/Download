ZEND_EXTERN_MODULE_GLOBALS(mbstring)

struct _zend_mb_regex_globals {
	OnigEncoding default_mbctype;
	OnigEncoding current_mbctype;
	HashTable ht_rc;
	zval *search_str;
	zval *search_str_val;
	unsigned int search_pos;
	php_mb_regex_t *search_re;
	OnigRegion *search_regs;
	OnigOptionType regex_default_options;
	OnigSyntaxType *regex_default_syntax;
};
