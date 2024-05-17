status_t OMXCodec::parseAVCCodecSpecificData(
 const void *data, size_t size,
 unsigned *profile, unsigned *level) {
 const uint8_t *ptr = (const uint8_t *)data;

 if (size < 7 || ptr[0] != 1) {
 return ERROR_MALFORMED;
 }

 *profile = ptr[1];
 *level = ptr[3];


 size_t lengthSize __unused = 1 + (ptr[4] & 3);


 size_t numSeqParameterSets = ptr[5] & 31;

    ptr += 6;
    size -= 6;

 for (size_t i = 0; i < numSeqParameterSets; ++i) {
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

 if (size < 1) {
 return ERROR_MALFORMED;
 }

 size_t numPictureParameterSets = *ptr;
 ++ptr;
 --size;

 for (size_t i = 0; i < numPictureParameterSets; ++i) {
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

 return OK;
}
