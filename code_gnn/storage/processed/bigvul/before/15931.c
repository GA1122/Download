VP8XChunk::VP8XChunk(Container* parent)
  : Chunk(parent, kChunk_VP8X)
{
    this->needsRewrite = true;
    this->size = 10;
    this->data.resize(this->size);
    this->data.assign(this->size, 0);
    XMP_Uns8* bitstream =
        (XMP_Uns8*)parent->chunks[WEBP_CHUNK_IMAGE][0]->data.data();
    XMP_Uns32 width = ((bitstream[7] << 8) | bitstream[6]) & 0x3fff;
    XMP_Uns32 height = ((bitstream[9] << 8) | bitstream[8]) & 0x3fff;
    this->width(width);
    this->height(height);
    parent->vp8x = this;
}
