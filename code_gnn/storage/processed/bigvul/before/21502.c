kadm5_randkey_principal_3(void *server_handle,
                          krb5_principal principal,
                          krb5_boolean keepold,
                          int n_ks_tuple, krb5_key_salt_tuple *ks_tuple,
                          krb5_keyblock **keyblocks,
                          int *n_keys)
{
    krb5_db_entry               *kdb;
    osa_princ_ent_rec           adb;
    krb5_int32                  now;
    kadm5_policy_ent_rec        pol;
    int                         ret, last_pwd, have_pol = 0;
    kadm5_server_handle_t       handle = server_handle;
    krb5_keyblock               *act_mkey;

    if (keyblocks)
        *keyblocks = NULL;

    CHECK_HANDLE(server_handle);

     
    if (n_ks_tuple == 0) {
        ks_tuple = handle->params.keysalts;
        n_ks_tuple = handle->params.num_keysalts;
    }

    krb5_clear_error_message(handle->context);

    if (principal == NULL)
        return EINVAL;
    if (krb5_principal_compare(handle->context, principal, hist_princ)) {
         
        if (keepold)
            return KADM5_PROTECT_PRINCIPAL;
        n_ks_tuple = 1;
    }

    if ((ret = kdb_get_entry(handle, principal, &kdb, &adb)))
        return(ret);

    ret = krb5_dbe_find_act_mkey(handle->context, active_mkey_list, NULL,
                                 &act_mkey);
    if (ret)
        goto done;

    ret = krb5_dbe_crk(handle->context, act_mkey, ks_tuple, n_ks_tuple,
                       keepold, kdb);
    if (ret)
        goto done;

    kdb->attributes &= ~KRB5_KDB_REQUIRES_PWCHANGE;

    ret = krb5_timeofday(handle->context, &now);
    if (ret)
        goto done;

    if ((adb.aux_attributes & KADM5_POLICY)) {
        if ((ret = kadm5_get_policy(handle->lhandle, adb.policy,
                                    &pol)) != KADM5_OK)
            goto done;
        have_pol = 1;

        ret = krb5_dbe_lookup_last_pwd_change(handle->context, kdb, &last_pwd);
        if (ret)
            goto done;

#if 0
         
        if((now - last_pwd) < pol.pw_min_life &&
           !(kdb->attributes & KRB5_KDB_REQUIRES_PWCHANGE)) {
            ret = KADM5_PASS_TOOSOON;
            goto done;
        }
#endif

        if (pol.pw_max_life)
            kdb->pw_expiration = now + pol.pw_max_life;
        else
            kdb->pw_expiration = 0;
    } else {
        kdb->pw_expiration = 0;
    }

    ret = krb5_dbe_update_last_pwd_change(handle->context, kdb, now);
    if (ret)
        goto done;

     
    kdb->fail_auth_count = 0;

    if (keyblocks) {
        ret = decrypt_key_data(handle->context,
                               kdb->n_key_data, kdb->key_data,
                               keyblocks, n_keys);
        if (ret)
            goto done;
    }

     
    kdb->mask = KADM5_KEY_DATA | KADM5_FAIL_AUTH_COUNT;
     ;

    ret = k5_kadm5_hook_chpass(handle->context, handle->hook_handles,
                               KADM5_HOOK_STAGE_PRECOMMIT, principal, keepold,
                               n_ks_tuple, ks_tuple, NULL);
    if (ret)
        goto done;
    if ((ret = kdb_put_entry(handle, kdb, &adb)))
        goto done;

    (void) k5_kadm5_hook_chpass(handle->context, handle->hook_handles,
                                KADM5_HOOK_STAGE_POSTCOMMIT, principal,
                                keepold, n_ks_tuple, ks_tuple, NULL);
    ret = KADM5_OK;
done:
    kdb_free_entry(handle, kdb, &adb);
    if (have_pol)
        kadm5_free_policy_ent(handle->lhandle, &pol);

    return ret;
}