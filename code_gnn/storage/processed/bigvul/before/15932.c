VP8XChunk::VP8XChunk(Container* parent, WEBP_MetaHandler* handler)
  : Chunk(parent, handler)
{
    this->size = 10;
    this->needsRewrite = true;
    parent->vp8x = this;
}
