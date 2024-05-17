ZEND_INI_MH(phar_ini_modify_handler)  
{
	zend_bool old, ini;

	if (ZSTR_LEN(entry->name) == sizeof("phar.readonly")-1) {
		old = PHAR_G(readonly_orig);
	} else {
		old = PHAR_G(require_hash_orig);
	}

	if (ZSTR_LEN(new_value) == 2 && !strcasecmp("on", ZSTR_VAL(new_value))) {
		ini = (zend_bool) 1;
	}
	else if (ZSTR_LEN(new_value) == 3 && !strcasecmp("yes", ZSTR_VAL(new_value))) {
		ini = (zend_bool) 1;
	}
	else if (ZSTR_LEN(new_value) == 4 && !strcasecmp("true", ZSTR_VAL(new_value))) {
		ini = (zend_bool) 1;
	}
	else {
		ini = (zend_bool) atoi(ZSTR_VAL(new_value));
	}

	 
	if (stage == ZEND_INI_STAGE_STARTUP) {
		if (ZSTR_LEN(entry->name) == sizeof("phar.readonly")-1) {
			PHAR_G(readonly_orig) = ini;
		} else {
			PHAR_G(require_hash_orig) = ini;
		}
	} else if (old && !ini) {
		return FAILURE;
	}

	if (ZSTR_LEN(entry->name) == sizeof("phar.readonly")-1) {
		PHAR_G(readonly) = ini;
		if (PHAR_G(request_init) && PHAR_G(phar_fname_map.u.flags)) {
			zend_hash_apply_with_argument(&(PHAR_G(phar_fname_map)), phar_set_writeable_bit, (void *)&ini);
		}
	} else {
		PHAR_G(require_hash) = ini;
	}

	return SUCCESS;
}
 
