status_t Parcel::writeByteArray(size_t len, const uint8_t *val) {
 if (len > INT32_MAX) {
 return BAD_VALUE;
 }

 if (!val) {
 return writeInt32(-1);
 }
 status_t ret = writeInt32(static_cast<uint32_t>(len));
 if (ret == NO_ERROR) {
        ret = write(val, len * sizeof(*val));
 }
 return ret;
}
