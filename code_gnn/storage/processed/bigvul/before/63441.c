select_client_key(krb5_context context, krb5_db_entry *client,
                  krb5_enctype *req_enctypes, int n_req_enctypes,
                  krb5_keyblock *kb_out, krb5_key_data **kd_out)
{
    krb5_error_code ret;
    krb5_key_data *kd;
    krb5_enctype etype;
    int i;

    memset(kb_out, 0, sizeof(*kb_out));
    *kd_out = NULL;

    for (i = 0; i < n_req_enctypes; i++) {
        etype = req_enctypes[i];
        if (!krb5_c_valid_enctype(etype))
            continue;
        if (krb5_dbe_find_enctype(context, client, etype, -1, 0, &kd) == 0) {
             
            ret = krb5_dbe_decrypt_key_data(context, NULL, kd, kb_out, NULL);
            if (ret)
                return ret;
            kb_out->enctype = etype;
            *kd_out = kd;
            return 0;
        }
    }
    return 0;
}