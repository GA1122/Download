GraphicBuffer::GraphicBuffer(uint32_t w, uint32_t h,
 PixelFormat inFormat, uint32_t inUsage,
 uint32_t inStride, native_handle_t* inHandle, bool keepOwnership)
 : BASE(), mOwner(keepOwnership ? ownHandle : ownNone),
      mBufferMapper(GraphicBufferMapper::get()),
      mInitCheck(NO_ERROR), mId(getUniqueId())
{
    width  = w;
    height = h;
    stride = inStride;
    format = inFormat;
    usage  = inUsage;
    handle = inHandle;
}
