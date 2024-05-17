status_t ESDS::parse() {
 uint8_t tag;
 size_t data_offset;
 size_t data_size;
 status_t err =
        skipDescriptorHeader(0, mSize, &tag, &data_offset, &data_size);

 if (err != OK) {
 return err;
 }

 if (tag != kTag_ESDescriptor) {
 return ERROR_MALFORMED;
 }

 return parseESDescriptor(data_offset, data_size);
}
