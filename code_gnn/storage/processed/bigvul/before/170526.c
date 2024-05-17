status_t Parcel::writeUnpadded(const void* data, size_t len)
{
 size_t end = mDataPos + len;
 if (end < mDataPos) {
 return BAD_VALUE;
 }

 if (end <= mDataCapacity) {
restart_write:
        memcpy(mData+mDataPos, data, len);
 return finishWrite(len);
 }

 status_t err = growData(len);
 if (err == NO_ERROR) goto restart_write;
 return err;
}
