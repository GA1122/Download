AMediaFormat* AMediaCodec_getOutputFormat(AMediaCodec *mData) {
    sp<AMessage> format;
    mData->mCodec->getOutputFormat(&format);
 return AMediaFormat_fromMsg(&format);
}
