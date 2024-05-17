media_status_t AMediaCodec_releaseCrypto(AMediaCodec *mData) {
 return translate_error(mData->mCodec->releaseCrypto());
}
