status_t ESDS::parseDecoderConfigDescriptor(size_t offset, size_t size) {
 if (size < 13) {
 return ERROR_MALFORMED;
 }

    mObjectTypeIndication = mData[offset];

    offset += 13;
    size -= 13;

 if (size == 0) {
        mDecoderSpecificOffset = 0;
        mDecoderSpecificLength = 0;
 return OK;
 }

 uint8_t tag;
 size_t sub_offset, sub_size;
 status_t err = skipDescriptorHeader(
            offset, size, &tag, &sub_offset, &sub_size);

 if (err != OK) {
 return err;
 }

 if (tag != kTag_DecoderSpecificInfo) {
 return ERROR_MALFORMED;
 }

    mDecoderSpecificOffset = sub_offset;
    mDecoderSpecificLength = sub_size;

 return OK;
}
