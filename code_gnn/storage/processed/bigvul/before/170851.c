ANativeWindowBuffer* GraphicBuffer::getNativeBuffer() const
{
    LOG_ALWAYS_FATAL_IF(this == NULL, "getNativeBuffer() called on NULL GraphicBuffer");
 return static_cast<ANativeWindowBuffer*>(
 const_cast<GraphicBuffer*>(this));
}
