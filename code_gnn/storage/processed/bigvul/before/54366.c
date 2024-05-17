asn1_decode_sequence_of_keys(krb5_data *in, ldap_seqof_key_data *out)
{
    krb5_error_code err;
    ldap_seqof_key_data *p;
    int i;

    memset(out, 0, sizeof(*out));

     
    err = kldap_ensure_initialized ();
    if (err)
        return err;

    err = accessor.asn1_ldap_decode_sequence_of_keys(in, &p);
    if (err)
        return err;

     
    for (i = 0; i < p->n_key_data; i++) {
        p->key_data[i].key_data_kvno = p->kvno;
         
        if (p->key_data[i].key_data_ver == 0)
            p->key_data[i].key_data_ver = 2;
    }

    *out = *p;
    free(p);
    return 0;
}
