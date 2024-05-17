int CMS_decrypt_set1_pkey(CMS_ContentInfo *cms, EVP_PKEY *pk, X509 *cert)
{
    STACK_OF(CMS_RecipientInfo) *ris;
    CMS_RecipientInfo *ri;
    int i, r, ri_type;
    int debug = 0, match_ri = 0;
    ris = CMS_get0_RecipientInfos(cms);
    if (ris)
        debug = cms->d.envelopedData->encryptedContentInfo->debug;
    ri_type = cms_pkey_get_ri_type(pk);
    if (ri_type == CMS_RECIPINFO_NONE) {
        CMSerr(CMS_F_CMS_DECRYPT_SET1_PKEY,
               CMS_R_NOT_SUPPORTED_FOR_THIS_KEY_TYPE);
        return 0;
    }

    for (i = 0; i < sk_CMS_RecipientInfo_num(ris); i++) {
        ri = sk_CMS_RecipientInfo_value(ris, i);
        if (CMS_RecipientInfo_type(ri) != ri_type)
            continue;
        match_ri = 1;
        if (ri_type == CMS_RECIPINFO_AGREE) {
            r = cms_kari_set1_pkey(cms, ri, pk, cert);
            if (r > 0)
                return 1;
            if (r < 0)
                return 0;
        }
         
        else if (!cert || !CMS_RecipientInfo_ktri_cert_cmp(ri, cert)) {
            CMS_RecipientInfo_set0_pkey(ri, pk);
            r = CMS_RecipientInfo_decrypt(cms, ri);
            CMS_RecipientInfo_set0_pkey(ri, NULL);
            if (cert) {
                 
                if (!debug) {
                    ERR_clear_error();
                    return 1;
                }
                if (r > 0)
                    return 1;
                CMSerr(CMS_F_CMS_DECRYPT_SET1_PKEY, CMS_R_DECRYPT_ERROR);
                return 0;
            }
             
            else if (r > 0 && debug)
                return 1;
        }
    }
     
    if (match_ri && !cert && !debug) {
        ERR_clear_error();
        return 1;
    }

    CMSerr(CMS_F_CMS_DECRYPT_SET1_PKEY, CMS_R_NO_MATCHING_RECIPIENT);
    return 0;

}