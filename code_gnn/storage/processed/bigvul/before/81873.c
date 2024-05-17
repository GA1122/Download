int wc_ecc_export_private_raw(ecc_key* key, byte* qx, word32* qxLen,
                              byte* qy, word32* qyLen, byte* d, word32* dLen)
{
     
    if (d == NULL || dLen == NULL)
        return BAD_FUNC_ARG;

    return wc_ecc_export_raw(key, qx, qxLen, qy, qyLen, d, dLen);
}
