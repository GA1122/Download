media_status_t AMediaCodecCryptoInfo_delete(AMediaCodecCryptoInfo* info) {
    free(info);
 return AMEDIA_OK;
}
