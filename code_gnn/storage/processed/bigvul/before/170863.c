status_t GraphicBuffer::reallocate(uint32_t w, uint32_t h, PixelFormat f,
 uint32_t reqUsage)
{
 if (mOwner != ownData)
 return INVALID_OPERATION;

 if (handle && w==width && h==height && f==format && reqUsage==usage)
 return NO_ERROR;

 if (handle) {
 GraphicBufferAllocator& allocator(GraphicBufferAllocator::get());
        allocator.free(handle);
        handle = 0;
 }
 return initSize(w, h, f, reqUsage);
}
