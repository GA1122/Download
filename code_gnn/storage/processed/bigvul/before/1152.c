GfxTilingPattern::~GfxTilingPattern() {
  resDict.free();
  contentStream.free();
}
