ssize_t AMediaCodec_dequeueInputBuffer(AMediaCodec *mData, int64_t timeoutUs) {
 size_t idx;
 status_t ret = mData->mCodec->dequeueInputBuffer(&idx, timeoutUs);
    requestActivityNotification(mData);
 if (ret == OK) {
 return idx;
 }
 return translate_error(ret);
}
