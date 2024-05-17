status_t Parcel::setData(const uint8_t* buffer, size_t len)
{
 status_t err = restartWrite(len);
 if (err == NO_ERROR) {
        memcpy(const_cast<uint8_t*>(data()), buffer, len);
        mDataSize = len;
        mFdsKnown = false;
 }
 return err;
}
