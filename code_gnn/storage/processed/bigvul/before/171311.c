void OMXCodec::addCodecSpecificData(const void *data, size_t size) {
 CodecSpecificData *specific =
 (CodecSpecificData *)malloc(sizeof(CodecSpecificData) + size - 1);

    specific->mSize = size;
    memcpy(specific->mData, data, size);

    mCodecSpecificData.push(specific);
}
