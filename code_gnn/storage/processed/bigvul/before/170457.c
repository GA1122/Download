status_t Parcel::read(void* outData, size_t len) const
{
 if ((mDataPos+PAD_SIZE(len)) >= mDataPos && (mDataPos+PAD_SIZE(len)) <= mDataSize
 && len <= PAD_SIZE(len)) {
        memcpy(outData, mData+mDataPos, len);
        mDataPos += PAD_SIZE(len);
        ALOGV("read Setting data pos of %p to %zu", this, mDataPos);
 return NO_ERROR;
 }
 return NOT_ENOUGH_DATA;
}
