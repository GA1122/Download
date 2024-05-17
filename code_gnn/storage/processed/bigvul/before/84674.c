kadm5_get_principal_keys(void *server_handle  ,
                         krb5_principal principal  ,
                         krb5_kvno kvno  ,
                         kadm5_key_data **key_data_out  ,
                         int *n_key_data_out  )
{
    krb5_db_entry               *kdb;
    osa_princ_ent_rec           adb;
    kadm5_ret_t                 ret;
    kadm5_server_handle_t       handle = server_handle;
    kadm5_key_data              *key_data = NULL;
    int i, nkeys = 0;

    if (principal == NULL || key_data_out == NULL || n_key_data_out == NULL)
        return EINVAL;

    CHECK_HANDLE(server_handle);

    if ((ret = kdb_get_entry(handle, principal, &kdb, &adb)))
        return(ret);

    key_data = calloc(kdb->n_key_data, sizeof(kadm5_key_data));
    if (key_data == NULL) {
        ret = ENOMEM;
        goto done;
    }

    for (i = 0, nkeys = 0; i < kdb->n_key_data; i++) {
        if (kvno != 0 && kvno != kdb->key_data[i].key_data_kvno)
            continue;
        key_data[nkeys].kvno = kdb->key_data[i].key_data_kvno;

        ret = krb5_dbe_decrypt_key_data(handle->context, NULL,
                                        &kdb->key_data[i],
                                        &key_data[nkeys].key,
                                        &key_data[nkeys].salt);
        if (ret)
            goto done;
        nkeys++;
    }

    *n_key_data_out = nkeys;
    *key_data_out = key_data;
    key_data = NULL;
    nkeys = 0;
    ret = KADM5_OK;

done:
    kdb_free_entry(handle, kdb, &adb);
    kadm5_free_kadm5_key_data(handle->context, nkeys, key_data);

    return ret;
}