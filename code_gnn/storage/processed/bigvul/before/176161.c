AMediaCodec* AMediaCodec_createEncoderByType(const char *name) {
 return createAMediaCodec(name, true, true);
}
