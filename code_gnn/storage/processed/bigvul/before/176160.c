AMediaCodec* AMediaCodec_createDecoderByType(const char *mime_type) {
 return createAMediaCodec(mime_type, true, false);
}
