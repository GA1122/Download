status_t ESDS::skipDescriptorHeader(
 size_t offset, size_t size,
 uint8_t *tag, size_t *data_offset, size_t *data_size) const {
 if (size == 0) {
 return ERROR_MALFORMED;
 }

 *tag = mData[offset++];
 --size;

 *data_size = 0;
 bool more;
 do {
 if (size == 0) {
 return ERROR_MALFORMED;
 }

 uint8_t x = mData[offset++];
 --size;

 *data_size = (*data_size << 7) | (x & 0x7f);
        more = (x & 0x80) != 0;
 }
 while (more);

    ALOGV("tag=0x%02x data_size=%zu", *tag, *data_size);

 if (*data_size > size) {
 return ERROR_MALFORMED;
 }

 *data_offset = offset;

 return OK;
}
