cms_contentinfo_create(krb5_context context,                           
                       pkinit_plg_crypto_context plg_cryptoctx,        
                       pkinit_req_crypto_context req_cryptoctx,        
                       pkinit_identity_crypto_context id_cryptoctx,    
                       int cms_msg_type,
                       unsigned char *data, unsigned int data_len,
                       unsigned char **out_data, unsigned int *out_data_len)
{
    krb5_error_code retval = ENOMEM;
    ASN1_OBJECT *oid;
    PKCS7 *p7 = NULL;
    unsigned char *p;

     
    oid = pkinit_pkcs7type2oid(plg_cryptoctx, cms_msg_type);
    if (oid == NULL)
        goto cleanup;
    retval = create_contentinfo(context, oid, data, data_len, &p7);
    if (retval != 0)
        goto cleanup;
    *out_data_len = i2d_PKCS7(p7, NULL);
    if (!(*out_data_len)) {
        retval = oerr(context, 0, _("Failed to DER encode PKCS7"));
        goto cleanup;
    }
    retval = ENOMEM;
    if ((p = *out_data = malloc(*out_data_len)) == NULL)
        goto cleanup;

     
    retval = i2d_PKCS7(p7, &p);
    if (!retval) {
        retval = oerr(context, 0, _("Failed to DER encode PKCS7"));
        goto cleanup;
    }
    retval = 0;
cleanup:
    if (p7)
        PKCS7_free(p7);
    return retval;
}