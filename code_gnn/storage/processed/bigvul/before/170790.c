status_t ESDS::getCodecSpecificInfo(const void **data, size_t *size) const {
 if (mInitCheck != OK) {
 return mInitCheck;
 }

 *data = &mData[mDecoderSpecificOffset];
 *size = mDecoderSpecificLength;

 return OK;
}
