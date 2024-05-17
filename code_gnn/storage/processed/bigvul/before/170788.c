ESDS::ESDS(const void *data, size_t size)
 : mData(new uint8_t[size]),
      mSize(size),
      mInitCheck(NO_INIT),
      mDecoderSpecificOffset(0),
      mDecoderSpecificLength(0),
      mObjectTypeIndication(0) {
    memcpy(mData, data, size);

    mInitCheck = parse();
}
