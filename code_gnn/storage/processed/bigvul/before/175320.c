status_t MPEG4Source::init() {
 if (mFirstMoofOffset != 0) {
 off64_t offset = mFirstMoofOffset;
 return parseChunk(&offset);
 }
 return OK;
}
