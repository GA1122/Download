crypto_cert_select(krb5_context context,
                   pkinit_cert_matching_data *md)
{
    struct _pkinit_cert_data *cd;
    if (md == NULL)
        return EINVAL;

    cd = (struct _pkinit_cert_data *)md->ch;
    if (cd == NULL || cd->magic != CERT_MAGIC)
        return EINVAL;

     
    if (cd->idctx->my_certs != NULL) {
        sk_X509_pop_free(cd->idctx->my_certs, X509_free);
    }
    cd->idctx->my_certs = sk_X509_new_null();
    sk_X509_push(cd->idctx->my_certs, cd->cred->cert);
    free(cd->idctx->identity);
     
    if (cd->idctx->creds[cd->index]->name != NULL)
        cd->idctx->identity = strdup(cd->idctx->creds[cd->index]->name);
    else
        cd->idctx->identity = NULL;
    cd->idctx->creds[cd->index]->cert = NULL;        
    cd->idctx->cert_index = 0;

    if (cd->idctx->pkcs11_method != 1) {
        cd->idctx->my_key = cd->cred->key;
        cd->idctx->creds[cd->index]->key = NULL;     
    }
#ifndef WITHOUT_PKCS11
    else {
        cd->idctx->cert_id = cd->cred->cert_id;
        cd->idctx->creds[cd->index]->cert_id = NULL;  
        cd->idctx->cert_id_len = cd->cred->cert_id_len;
    }
#endif
    return 0;
}
