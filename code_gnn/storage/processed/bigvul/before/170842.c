GraphicBuffer::GraphicBuffer()
 : BASE(), mOwner(ownData), mBufferMapper(GraphicBufferMapper::get()),
      mInitCheck(NO_ERROR), mId(getUniqueId())
{
    width  = 
    height = 
    stride = 
    format = 
    usage  = 0;
    handle = NULL;
}
