static int cms_RecipientInfo_ktri_init(CMS_RecipientInfo *ri, X509 *recip,
                                       EVP_PKEY *pk, unsigned int flags)
{
    CMS_KeyTransRecipientInfo *ktri;
    int idtype;

    ri->d.ktri = M_ASN1_new_of(CMS_KeyTransRecipientInfo);
    if (!ri->d.ktri)
        return 0;
    ri->type = CMS_RECIPINFO_TRANS;

    ktri = ri->d.ktri;

    if (flags & CMS_USE_KEYID) {
        ktri->version = 2;
        idtype = CMS_RECIPINFO_KEYIDENTIFIER;
    } else {
        ktri->version = 0;
        idtype = CMS_RECIPINFO_ISSUER_SERIAL;
    }

     

    if (!cms_set1_SignerIdentifier(ktri->rid, recip, idtype))
        return 0;

    CRYPTO_add(&recip->references, 1, CRYPTO_LOCK_X509);
    CRYPTO_add(&pk->references, 1, CRYPTO_LOCK_EVP_PKEY);
    ktri->pkey = pk;
    ktri->recip = recip;

    if (flags & CMS_KEY_PARAM) {
        ktri->pctx = EVP_PKEY_CTX_new(ktri->pkey, NULL);
        if (!ktri->pctx)
            return 0;
        if (EVP_PKEY_encrypt_init(ktri->pctx) <= 0)
            return 0;
    } else if (!cms_env_asn1_ctrl(ri, 0))
        return 0;
    return 1;
}
