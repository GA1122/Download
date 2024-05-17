const void* Parcel::readInplace(size_t len) const
{
 if ((mDataPos+PAD_SIZE(len)) >= mDataPos && (mDataPos+PAD_SIZE(len)) <= mDataSize
 && len <= PAD_SIZE(len)) {
 const void* data = mData+mDataPos;
        mDataPos += PAD_SIZE(len);
        ALOGV("readInplace Setting data pos of %p to %zu", this, mDataPos);
 return data;
 }
 return NULL;
}
