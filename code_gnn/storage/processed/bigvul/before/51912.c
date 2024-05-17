AirPDcapRsnaPwd2PskStep(
    const guint8 *ppBytes,
    const guint ppLength,
    const CHAR *ssid,
    const size_t ssidLength,
    const INT iterations,
    const INT count,
    UCHAR *output)
{
    UCHAR digest[MAX_SSID_LENGTH+4];   
    UCHAR digest1[SHA1_DIGEST_LEN];
    INT i, j;

    if (ssidLength > MAX_SSID_LENGTH) {
         
        return AIRPDCAP_RET_UNSUCCESS;
    }

    memset(digest, 0, sizeof digest);
    memset(digest1, 0, sizeof digest1);

     
    memcpy(digest, ssid, ssidLength);
    digest[ssidLength] = (UCHAR)((count>>24) & 0xff);
    digest[ssidLength+1] = (UCHAR)((count>>16) & 0xff);
    digest[ssidLength+2] = (UCHAR)((count>>8) & 0xff);
    digest[ssidLength+3] = (UCHAR)(count & 0xff);
    sha1_hmac(ppBytes, ppLength, digest, (guint32) ssidLength+4, digest1);

     
    memcpy(output, digest1, SHA1_DIGEST_LEN);
    for (i = 1; i < iterations; i++) {
         
        sha1_hmac(ppBytes, ppLength, digest1, SHA1_DIGEST_LEN, digest);

        memcpy(digest1, digest, SHA1_DIGEST_LEN);
         
        for (j = 0; j < SHA1_DIGEST_LEN; j++) {
            output[j] ^= digest[j];
        }
    }

    return AIRPDCAP_RET_SUCCESS;
}
