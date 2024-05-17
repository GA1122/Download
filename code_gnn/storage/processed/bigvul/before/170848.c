void GraphicBuffer::free_handle()
{
 if (mOwner == ownHandle) {
        mBufferMapper.unregisterBuffer(handle);
        native_handle_close(handle);
        native_handle_delete(const_cast<native_handle*>(handle));
 } else if (mOwner == ownData) {
 GraphicBufferAllocator& allocator(GraphicBufferAllocator::get());
        allocator.free(handle);
 }
    mWrappedBuffer = 0;
}
