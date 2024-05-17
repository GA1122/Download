int cms_pkey_get_ri_type(EVP_PKEY *pk)
{
    if (pk->ameth && pk->ameth->pkey_ctrl) {
        int i, r;
        i = pk->ameth->pkey_ctrl(pk, ASN1_PKEY_CTRL_CMS_RI_TYPE, 0, &r);
        if (i > 0)
            return r;
    }
    return CMS_RECIPINFO_TRANS;
}
