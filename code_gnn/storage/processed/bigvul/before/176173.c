media_status_t AMediaCodec_queueInputBuffer(AMediaCodec *mData,
 size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags) {

 AString errorMsg;
 status_t ret = mData->mCodec->queueInputBuffer(idx, offset, size, time, flags, &errorMsg);
 return translate_error(ret);
}
