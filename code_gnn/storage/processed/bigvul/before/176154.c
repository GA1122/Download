cryptoinfo_mode_t AMediaCodecCryptoInfo_getMode(AMediaCodecCryptoInfo* ci) {
 if (!ci) {
 return (cryptoinfo_mode_t) AMEDIA_ERROR_INVALID_OBJECT;
 }
 return ci->mode;
}
