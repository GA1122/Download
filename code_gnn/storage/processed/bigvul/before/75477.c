kdc_handle_protected_negotiation(krb5_context context,
                                 krb5_data *req_pkt, krb5_kdc_req *request,
                                 const krb5_keyblock *reply_key,
                                 krb5_pa_data ***out_enc_padata)
{
    krb5_error_code retval = 0;
    krb5_checksum checksum;
    krb5_data *der_cksum = NULL;
    krb5_pa_data *pa, *pa_in;

    memset(&checksum, 0, sizeof(checksum));

    pa_in = krb5int_find_pa_data(context, request->padata,
                                 KRB5_ENCPADATA_REQ_ENC_PA_REP);
    if (pa_in == NULL)
        return 0;

     
    retval = krb5_c_make_checksum(context, 0, reply_key, KRB5_KEYUSAGE_AS_REQ,
                                  req_pkt, &checksum);
    if (retval != 0)
        goto cleanup;
    retval = encode_krb5_checksum(&checksum, &der_cksum);
    if (retval != 0)
        goto cleanup;

     
    retval = alloc_pa_data(KRB5_ENCPADATA_REQ_ENC_PA_REP, 0, &pa);
    if (retval)
        goto cleanup;
    pa->length = der_cksum->length;
    pa->contents = (uint8_t *)der_cksum->data;
    der_cksum->data = NULL;
     
    retval = add_pa_data_element(out_enc_padata, pa);
    if (retval)
        goto cleanup;

     
    retval = alloc_pa_data(KRB5_PADATA_FX_FAST, 0, &pa);
    if (retval)
        goto cleanup;
     
    retval = add_pa_data_element(out_enc_padata, pa);

cleanup:
    krb5_free_checksum_contents(context, &checksum);
    krb5_free_data(context, der_cksum);
    return retval;
}