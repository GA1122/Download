XMP_Uns32 VP8XChunk::width()
{
    return GetLE24(&this->data[4]) + 1;
 }
