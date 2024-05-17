status_t OMXCodec::parseHEVCCodecSpecificData(
 const void *data, size_t size,
 unsigned *profile, unsigned *level) {
 const uint8_t *ptr = (const uint8_t *)data;

 if (size < 7 || ptr[0] != 1) {
 return ERROR_MALFORMED;
 }

 *profile = (ptr[1] & 31);
 *level = ptr[12];

    ptr += 22;
    size -= 22;

 size_t numofArrays = (char)ptr[0];
    ptr += 1;
    size -= 1;
 size_t j = 0, i = 0;
 for (i = 0; i < numofArrays; i++) {
        ptr += 1;
        size -= 1;

 size_t numofNals = U16_AT(ptr);
        ptr += 2;
        size -= 2;

 for (j = 0;j < numofNals;j++) {
 if (size < 2) {
 return ERROR_MALFORMED;
 }

 size_t length = U16_AT(ptr);

            ptr += 2;
            size -= 2;

 if (size < length) {
 return ERROR_MALFORMED;
 }
            addCodecSpecificData(ptr, length);

            ptr += length;
            size -= length;
 }
 }
 return OK;
}
