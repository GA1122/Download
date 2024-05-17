int create_history_entry(krb5_context context,
                         krb5_keyblock *hist_key, int n_key_data,
                         krb5_key_data *key_data, osa_pw_hist_ent *hist_out)
{
    int i;
    krb5_error_code ret = 0;
    krb5_keyblock key;
    krb5_keysalt salt;
    krb5_ui_2 kvno;
    osa_pw_hist_ent hist;

    hist_out->key_data = NULL;
    hist_out->n_key_data = 0;

    if (n_key_data < 0)
        return EINVAL;

    memset(&key, 0, sizeof(key));
    memset(&hist, 0, sizeof(hist));

    if (n_key_data == 0)
        goto cleanup;

    hist.key_data = k5calloc(n_key_data, sizeof(krb5_key_data), &ret);
    if (hist.key_data == NULL)
        goto cleanup;

     
    kvno = key_data[0].key_data_kvno;

    for (i = 0; i < n_key_data; i++) {
        if (key_data[i].key_data_kvno < kvno)
            break;
        ret = krb5_dbe_decrypt_key_data(context, NULL,
                                        &key_data[i], &key,
                                        &salt);
        if (ret)
            goto cleanup;

        ret = krb5_dbe_encrypt_key_data(context, hist_key, &key, &salt,
                                        key_data[i].key_data_kvno,
                                        &hist.key_data[hist.n_key_data]);
        if (ret)
            goto cleanup;
        hist.n_key_data++;
        krb5_free_keyblock_contents(context, &key);
         
    }

    *hist_out = hist;
    hist.n_key_data = 0;
    hist.key_data = NULL;

cleanup:
    krb5_free_keyblock_contents(context, &key);
    free_history_entry(context, &hist);
    return ret;
}