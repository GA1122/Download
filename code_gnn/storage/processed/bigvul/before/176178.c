media_status_t AMediaCodec_releaseOutputBufferAtTime(
 AMediaCodec *mData, size_t idx, int64_t timestampNs) {
    ALOGV("render @ %" PRId64, timestampNs);
 return translate_error(mData->mCodec->renderOutputBufferAndRelease(idx, timestampNs));
}
