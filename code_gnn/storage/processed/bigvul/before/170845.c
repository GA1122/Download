GraphicBuffer::GraphicBuffer(ANativeWindowBuffer* buffer, bool keepOwnership)
 : BASE(), mOwner(keepOwnership ? ownHandle : ownNone),
      mBufferMapper(GraphicBufferMapper::get()),
      mInitCheck(NO_ERROR), mWrappedBuffer(buffer), mId(getUniqueId())
{
    width  = buffer->width;
    height = buffer->height;
    stride = buffer->stride;
    format = buffer->format;
    usage  = buffer->usage;
    handle = buffer->handle;
}
