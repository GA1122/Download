status_t Parcel::read(void* outData, size_t len) const
{
 if (len > INT32_MAX) {
 return BAD_VALUE;
 }

 if ((mDataPos+pad_size(len)) >= mDataPos && (mDataPos+pad_size(len)) <= mDataSize
 && len <= pad_size(len)) {
        memcpy(outData, mData+mDataPos, len);
        mDataPos += pad_size(len);
        ALOGV("read Setting data pos of %p to %zu", this, mDataPos);
 return NO_ERROR;
 }
 return NOT_ENOUGH_DATA;
}
