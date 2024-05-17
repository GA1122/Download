status_t Parcel::write(const void* data, size_t len)
{
 if (len > INT32_MAX) {
 return BAD_VALUE;
 }

 void* const d = writeInplace(len);
 if (d) {
        memcpy(d, data, len);
 return NO_ERROR;
 }
 return mError;
}
