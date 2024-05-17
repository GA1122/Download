media_status_t AMediaCodecCryptoInfo_getKey(AMediaCodecCryptoInfo* ci, uint8_t *dst) {
 if (!ci) {
 return AMEDIA_ERROR_INVALID_OBJECT;
 }
 if (!dst) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }
    memcpy(dst, ci->key, 16);
 return AMEDIA_OK;
}
