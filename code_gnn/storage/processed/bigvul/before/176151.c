media_status_t AMediaCodecCryptoInfo_getEncryptedBytes(AMediaCodecCryptoInfo* ci, size_t *dst) {
 if (!ci) {
 return AMEDIA_ERROR_INVALID_OBJECT;
 }
 if (!dst) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }
    memcpy(dst, ci->encryptedbytes, sizeof(size_t) * ci->numsubsamples);
 return AMEDIA_OK;
}
