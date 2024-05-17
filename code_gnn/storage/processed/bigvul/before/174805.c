status_t IGraphicBufferConsumer::BufferItem::unflatten(
 void const*& buffer, size_t& size, int const*& fds, size_t& count) {

 if (size < sizeof(uint32_t))
 return NO_MEMORY;

 uint32_t flags = 0;
 FlattenableUtils::read(buffer, size, flags);

 if (flags & 1) {
        mGraphicBuffer = new GraphicBuffer();
 status_t err = mGraphicBuffer->unflatten(buffer, size, fds, count);
 if (err) return err;
        size -= FlattenableUtils::align<4>(buffer);
 }

 if (flags & 2) {
        mFence = new Fence();
 status_t err = mFence->unflatten(buffer, size, fds, count);
 if (err) return err;
        size -= FlattenableUtils::align<4>(buffer);
 }

 if (size < getPodSize()) {
 return NO_MEMORY;
 }

 FlattenableUtils::read(buffer, size, mCrop);
 FlattenableUtils::read(buffer, size, mTransform);
 FlattenableUtils::read(buffer, size, mScalingMode);
 FlattenableUtils::read(buffer, size, mTimestamp);
    mIsAutoTimestamp = readBoolFromInt(buffer, size);
 FlattenableUtils::read(buffer, size, mFrameNumber);
 FlattenableUtils::read(buffer, size, mBuf);
    mIsDroppable = readBoolFromInt(buffer, size);
    mAcquireCalled = readBoolFromInt(buffer, size);
    mTransformToDisplayInverse = readBoolFromInt(buffer, size);

 return NO_ERROR;
}
