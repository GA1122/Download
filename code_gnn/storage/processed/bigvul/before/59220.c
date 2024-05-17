_kdc_find_etype(krb5_context context, krb5_boolean use_strongest_session_key,
		krb5_boolean is_preauth, hdb_entry_ex *princ,
		krb5_enctype *etypes, unsigned len,
		krb5_enctype *ret_enctype, Key **ret_key)
{
    krb5_error_code ret;
    krb5_salt def_salt;
    krb5_enctype enctype = (krb5_enctype)ETYPE_NULL;
    const krb5_enctype *p;
    Key *key = NULL;
    int i, k;

     
    ret = krb5_get_pw_salt(context, princ->entry.principal, &def_salt);
    if (ret)
	return ret;

    ret = KRB5KDC_ERR_ETYPE_NOSUPP;

    if (use_strongest_session_key) {

	 

	 
	p = krb5_kerberos_enctypes(context);
	for (i = 0;
	    p[i] != (krb5_enctype)ETYPE_NULL && enctype == (krb5_enctype)ETYPE_NULL;
	    i++) {
	    if (krb5_enctype_valid(context, p[i]) != 0 &&
                !_kdc_is_weak_exception(princ->entry.principal, p[i]))
		continue;

	     
	    for (k = 0; k < len && enctype == (krb5_enctype)ETYPE_NULL; k++) {

		if (p[i] != etypes[k])
		    continue;

                 
		key = NULL;
		while (hdb_next_enctype2key(context, &princ->entry, NULL,
					     p[i], &key) == 0) {
		    if (key->key.keyvalue.length == 0) {
			ret = KRB5KDC_ERR_NULL_KEY;
			continue;
		    }
		    enctype = p[i];
		    ret = 0;
		    if (is_preauth && ret_key != NULL &&
			!is_default_salt_p(&def_salt, key))
			continue;
		}
	    }
	}
    } else {
	 
	for(i = 0; ret != 0 && i < len; i++) {

	    if (krb5_enctype_valid(context, etypes[i]) != 0 &&
		!_kdc_is_weak_exception(princ->entry.principal, etypes[i]))
		continue;

	    key = NULL;
	    while (ret != 0 &&
                   hdb_next_enctype2key(context, &princ->entry, NULL,
					etypes[i], &key) == 0) {
		if (key->key.keyvalue.length == 0) {
		    ret = KRB5KDC_ERR_NULL_KEY;
		    continue;
		}
                enctype = etypes[i];
		ret = 0;
		if (is_preauth && ret_key != NULL &&
		    !is_default_salt_p(&def_salt, key))
		    continue;
	    }
	}
    }

    if (enctype == (krb5_enctype)ETYPE_NULL) {
         
	if (ret_key == NULL &&
	    _kdc_is_weak_exception(princ->entry.principal, ETYPE_DES_CBC_CRC)) {
            ret = 0;
            enctype = ETYPE_DES_CBC_CRC;
        } else {
            ret = KRB5KDC_ERR_ETYPE_NOSUPP;
        }
    }

    if (ret == 0) {
	if (ret_enctype != NULL)
	    *ret_enctype = enctype;
	if (ret_key != NULL)
	    *ret_key = key;
    }

    krb5_free_salt (context, def_salt);
    return ret;
}