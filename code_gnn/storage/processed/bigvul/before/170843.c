GraphicBuffer::GraphicBuffer(uint32_t w, uint32_t h, 
 PixelFormat reqFormat, uint32_t reqUsage)
 : BASE(), mOwner(ownData), mBufferMapper(GraphicBufferMapper::get()),
      mInitCheck(NO_ERROR), mId(getUniqueId())
{
    width  = 
    height = 
    stride = 
    format = 
    usage  = 0;
    handle = NULL;
    mInitCheck = initSize(w, h, reqFormat, reqUsage);
}
