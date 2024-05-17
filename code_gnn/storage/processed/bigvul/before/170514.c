status_t Parcel::writeInt32Array(size_t len, const int32_t *val) {
 if (!val) {
 return writeAligned(-1);
 }
 status_t ret = writeAligned(len);
 if (ret == NO_ERROR) {
        ret = write(val, len * sizeof(*val));
 }
 return ret;
}
