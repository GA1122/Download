const uint8_t *ID3::Iterator::getData(size_t *length) const {
 *length = 0;

 if (mFrameData == NULL) {
 return NULL;
 }

 if (mFrameSize < getHeaderLength()) {
 return NULL;
 }

 *length = mFrameSize - getHeaderLength();

 return mFrameData;
}
