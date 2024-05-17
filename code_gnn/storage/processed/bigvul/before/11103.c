ZEND_INI_MH(phar_ini_cache_list)  
{
	PHAR_G(cache_list) = ZSTR_VAL(new_value);

	if (stage == ZEND_INI_STAGE_STARTUP) {
		phar_split_cache_list();
	}

	return SUCCESS;
}
 
