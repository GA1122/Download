encode_keys(krb5_key_data *key_data_in, int n_key_data, krb5_kvno mkvno,
            struct berval **bval_out)
{
    krb5_error_code err = 0;
    int i;
    krb5_key_data *key_data = NULL;
    struct berval *bval = NULL;
    krb5_data *code;

    *bval_out = NULL;
    if (n_key_data <= 0) {
        err = EINVAL;
        goto cleanup;
    }

     
    key_data = k5calloc(n_key_data, sizeof(*key_data), &err);
    if (key_data == NULL)
        goto cleanup;
    memcpy(key_data, key_data_in, n_key_data * sizeof(*key_data));

     
    for (i = 0; i < n_key_data; i++) {
        if (key_data[i].key_data_ver == 1) {
            key_data[i].key_data_ver = 2;
            key_data[i].key_data_type[1] = KRB5_KDB_SALTTYPE_NORMAL;
            key_data[i].key_data_length[1] = 0;
            key_data[i].key_data_contents[1] = NULL;
        }
    }

    bval = k5alloc(sizeof(struct berval), &err);
    if (bval == NULL)
        goto cleanup;

    err = asn1_encode_sequence_of_keys(key_data, n_key_data, mkvno, &code);
    if (err)
        goto cleanup;

     
    bval->bv_len = code->length;
    bval->bv_val = code->data;
    free(code);

    *bval_out = bval;
    bval = NULL;

cleanup:
    free(key_data);
    free(bval);
    return err;
}
