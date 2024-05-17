AMediaFormat* AMediaCodec_getBufferFormat(AMediaCodec *mData, size_t index) {
    sp<AMessage> format;
    mData->mCodec->getOutputFormat(index, &format);
 return AMediaFormat_fromMsg(&format);
}
