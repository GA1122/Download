void VP8XChunk::xmp(bool hasXMP)
{
    XMP_Uns32 flags = GetLE32(&this->data[0]);
    flags ^= (-hasXMP ^ flags) & (1 << XMP_FLAG_BIT);
    PutLE32(&this->data[0], flags);
}
