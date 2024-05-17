kadm5_setkey_principal_4(void *server_handle, krb5_principal principal,
                         krb5_boolean keepold, kadm5_key_data *key_data,
                         int n_key_data)
{
    krb5_db_entry *kdb;
    osa_princ_ent_rec adb;
    krb5_timestamp now;
    kadm5_policy_ent_rec pol;
    krb5_key_data *new_key_data = NULL;
    int i, j, ret, n_new_key_data = 0;
    krb5_kvno kvno;
    krb5_boolean similar, have_pol = FALSE;
    kadm5_server_handle_t handle = server_handle;
    krb5_keyblock *act_mkey;
    krb5_key_salt_tuple *ks_from_keys = NULL;

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if (principal == NULL || key_data == NULL || n_key_data == 0)
        return EINVAL;

     
    if (hist_princ != NULL &&
        krb5_principal_compare(handle->context, principal, hist_princ))
        return KADM5_PROTECT_PRINCIPAL;

     
    kvno = key_data[0].kvno;
    for (i = 1; i < n_key_data; i++) {
        if (key_data[i].kvno != kvno)
            return KADM5_SETKEY_BAD_KVNO;
    }

    ret = kdb_get_entry(handle, principal, &kdb, &adb);
    if (ret)
        return ret;

    if (kvno == 0) {
         
        for (i = 0; i < kdb->n_key_data; i++) {
            if (kdb->key_data[i].key_data_kvno > kvno)
                kvno = kdb->key_data[i].key_data_kvno;
        }
        kvno++;
    } else if (keepold) {
         
        for (i = 0; i < kdb->n_key_data; i++) {
            if (kdb->key_data[i].key_data_kvno == kvno) {
                ret = KADM5_SETKEY_BAD_KVNO;
                goto done;
            }
        }
    }

    ret = make_ks_from_key_data(handle->context, key_data, n_key_data,
                                &ks_from_keys);
    if (ret)
        goto done;

    ret = apply_keysalt_policy(handle, adb.policy, n_key_data, ks_from_keys,
                               NULL, NULL);
    free(ks_from_keys);
    if (ret)
        goto done;

    for (i = 0; i < n_key_data; i++) {
        for (j = i + 1; j < n_key_data; j++) {
            ret = krb5_c_enctype_compare(handle->context,
                                         key_data[i].key.enctype,
                                         key_data[j].key.enctype,
                                         &similar);
            if (ret)
                goto done;
            if (similar) {
                if (key_data[i].salt.type == key_data[j].salt.type) {
                    ret = KADM5_SETKEY_DUP_ENCTYPES;
                    goto done;
                }
            }
        }
    }

    n_new_key_data = n_key_data + (keepold ? kdb->n_key_data : 0);
    new_key_data = calloc(n_new_key_data, sizeof(krb5_key_data));
    if (new_key_data == NULL) {
        n_new_key_data = 0;
        ret = ENOMEM;
        goto done;
    }

    n_new_key_data = 0;
    for (i = 0; i < n_key_data; i++) {

        ret = kdb_get_active_mkey(handle, NULL, &act_mkey);
        if (ret)
            goto done;

        ret = krb5_dbe_encrypt_key_data(handle->context, act_mkey,
                                        &key_data[i].key, &key_data[i].salt,
                                        kvno, &new_key_data[i]);
        if (ret)
            goto done;

        n_new_key_data++;
    }

     
    if (keepold) {
        memcpy(new_key_data + n_new_key_data, kdb->key_data,
               kdb->n_key_data * sizeof(krb5_key_data));
        memset(kdb->key_data, 0, kdb->n_key_data * sizeof(krb5_key_data));

         
        krb5_dbe_sort_key_data(new_key_data, n_new_key_data);
    }

     
    cleanup_key_data(handle->context, kdb->n_key_data, kdb->key_data);
    kdb->key_data = new_key_data;
    kdb->n_key_data = n_new_key_data;
    new_key_data = NULL;
    n_new_key_data = 0;

    kdb->attributes &= ~KRB5_KDB_REQUIRES_PWCHANGE;

    ret = krb5_timeofday(handle->context, &now);
    if (ret)
        goto done;

    if (adb.aux_attributes & KADM5_POLICY) {
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

    ret = kdb_put_entry(handle, kdb, &adb);
    if (ret)
        goto done;

    ret = KADM5_OK;

done:
    cleanup_key_data(handle->context, n_new_key_data, new_key_data);
    kdb_free_entry(handle, kdb, &adb);
    if (have_pol)
        kadm5_free_policy_ent(handle->lhandle, &pol);
    return ret;
}