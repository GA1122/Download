kadm5_setv4key_principal(void *server_handle,
                         krb5_principal principal,
                         krb5_keyblock *keyblock)
{
    krb5_db_entry               *kdb;
    osa_princ_ent_rec           adb;
    krb5_timestamp              now;
    kadm5_policy_ent_rec        pol;
    krb5_keysalt                keysalt;
    int                         i, kvno, ret;
    krb5_boolean                have_pol = FALSE;
    kadm5_server_handle_t       handle = server_handle;
    krb5_key_data               tmp_key_data;
    krb5_keyblock               *act_mkey;

    memset( &tmp_key_data, 0, sizeof(tmp_key_data));

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if (principal == NULL || keyblock == NULL)
        return EINVAL;
    if (hist_princ &&  
        ((krb5_principal_compare(handle->context,
                                 principal, hist_princ)) == TRUE))
        return KADM5_PROTECT_PRINCIPAL;

    if (keyblock->enctype != ENCTYPE_DES_CBC_CRC)
        return KADM5_SETV4KEY_INVAL_ENCTYPE;

    if ((ret = kdb_get_entry(handle, principal, &kdb, &adb)))
        return(ret);

    for (kvno = 0, i=0; i<kdb->n_key_data; i++)
        if (kdb->key_data[i].key_data_kvno > kvno)
            kvno = kdb->key_data[i].key_data_kvno;

    if (kdb->key_data != NULL)
        cleanup_key_data(handle->context, kdb->n_key_data, kdb->key_data);

    kdb->key_data = calloc(1, sizeof(krb5_key_data));
    if (kdb->key_data == NULL)
        return ENOMEM;
    kdb->n_key_data = 1;
    keysalt.type = KRB5_KDB_SALTTYPE_V4;
     
    keysalt.data.length = 0;
    keysalt.data.data = NULL;

    ret = kdb_get_active_mkey(handle, NULL, &act_mkey);
    if (ret)
        goto done;

     
    ret = krb5_dbe_encrypt_key_data(handle->context, act_mkey, keyblock,
                                    &keysalt, kvno + 1, kdb->key_data);
    if (ret) {
        goto done;
    }

    kdb->attributes &= ~KRB5_KDB_REQUIRES_PWCHANGE;

    ret = krb5_timeofday(handle->context, &now);
    if (ret)
        goto done;

    if ((adb.aux_attributes & KADM5_POLICY)) {
        ret = get_policy(handle, adb.policy, &pol, &have_pol);
        if (ret)
            goto done;
    }
    if (have_pol) {
        if (pol.pw_max_life)
            kdb->pw_expiration = ts_incr(now, pol.pw_max_life);
        else
            kdb->pw_expiration = 0;
    } else {
        kdb->pw_expiration = 0;
    }

    ret = krb5_dbe_update_last_pwd_change(handle->context, kdb, now);
    if (ret)
        goto done;

     
    kdb->fail_auth_count = 0;

     
    kdb->mask = KADM5_KEY_DATA | KADM5_FAIL_AUTH_COUNT;

    if ((ret = kdb_put_entry(handle, kdb, &adb)))
        goto done;

    ret = KADM5_OK;
done:
    for (i = 0; i < tmp_key_data.key_data_ver; i++) {
        if (tmp_key_data.key_data_contents[i]) {
            memset (tmp_key_data.key_data_contents[i], 0, tmp_key_data.key_data_length[i]);
            free (tmp_key_data.key_data_contents[i]);
        }
    }

    kdb_free_entry(handle, kdb, &adb);
    if (have_pol)
        kadm5_free_policy_ent(handle->lhandle, &pol);

    return ret;
}