ZEND_INI_MH(phar_ini_modify_handler)  
{
	zend_bool old, ini;

	if (entry->name_length == 14) {
		old = PHAR_G(readonly_orig);
	} else {
		old = PHAR_G(require_hash_orig);
	}

	if (new_value_length == 2 && !strcasecmp("on", new_value)) {
		ini = (zend_bool) 1;
	}
	else if (new_value_length == 3 && !strcasecmp("yes", new_value)) {
		ini = (zend_bool) 1;
	}
	else if (new_value_length == 4 && !strcasecmp("true", new_value)) {
		ini = (zend_bool) 1;
	}
	else {
		ini = (zend_bool) atoi(new_value);
	}

	 
	if (stage == ZEND_INI_STAGE_STARTUP) {
		if (entry->name_length == 14) {
			PHAR_G(readonly_orig) = ini;
		} else {
			PHAR_G(require_hash_orig) = ini;
		}
	} else if (old && !ini) {
		return FAILURE;
	}

	if (entry->name_length == 14) {
		PHAR_G(readonly) = ini;
		if (PHAR_GLOBALS->request_init && PHAR_GLOBALS->phar_fname_map.arBuckets) {
			zend_hash_apply_with_argument(&(PHAR_GLOBALS->phar_fname_map), phar_set_writeable_bit, (void *)&ini TSRMLS_CC);
		}
	} else {
		PHAR_G(require_hash) = ini;
	}

	return SUCCESS;
}
 
