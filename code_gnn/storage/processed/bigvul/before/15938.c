 bool VP8XChunk::xmp()
 {
    XMP_Uns32 flags = GetLE32(&this->data[0]);
    return (bool)((flags >> XMP_FLAG_BIT) & 1);
}
