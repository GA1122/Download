 XMP_Uns32 VP8XChunk::height()
 {
    return GetLE24(&this->data[7]) + 1;
 }
