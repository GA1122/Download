media_status_t AMediaCodec_flush(AMediaCodec *mData) {
 return translate_error(mData->mCodec->flush());
}
