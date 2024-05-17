status_t Parcel::setDataSize(size_t size)
{
 if (size > INT32_MAX) {
 return BAD_VALUE;
 }

 status_t err;
    err = continueWrite(size);
 if (err == NO_ERROR) {
        mDataSize = size;
        ALOGV("setDataSize Setting data size of %p to %zu", this, mDataSize);
 }
 return err;
}
