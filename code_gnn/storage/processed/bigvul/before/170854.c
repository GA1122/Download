status_t GraphicBuffer::initSize(uint32_t w, uint32_t h, PixelFormat format,
 uint32_t reqUsage)
{
 GraphicBufferAllocator& allocator = GraphicBufferAllocator::get();
 status_t err = allocator.alloc(w, h, format, reqUsage, &handle, &stride);
 if (err == NO_ERROR) {
 this->width  = w;
 this->height = h;
 this->format = format;
 this->usage  = reqUsage;
 }
 return err;
}
