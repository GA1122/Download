status_t IGraphicBufferConsumer::BufferItem::flatten(
 void*& buffer, size_t& size, int*& fds, size_t& count) const {

 if (size < BufferItem::getFlattenedSize()) {
 return NO_MEMORY;
 }

 uint32_t& flags = *static_cast<uint32_t*>(buffer);

 FlattenableUtils::advance(buffer, size, sizeof(uint32_t));

    flags = 0;
 if (mGraphicBuffer != 0) {
 status_t err = mGraphicBuffer->flatten(buffer, size, fds, count);
 if (err) return err;
        size -= FlattenableUtils::align<4>(buffer);
        flags |= 1;
 }
 if (mFence != 0) {
 status_t err = mFence->flatten(buffer, size, fds, count);
 if (err) return err;
        size -= FlattenableUtils::align<4>(buffer);
        flags |= 2;
 }

 if (size < getPodSize()) {
 return NO_MEMORY;
 }

 FlattenableUtils::write(buffer, size, mCrop);
 FlattenableUtils::write(buffer, size, mTransform);
 FlattenableUtils::write(buffer, size, mScalingMode);
 FlattenableUtils::write(buffer, size, mTimestamp);
    writeBoolAsInt(buffer, size, mIsAutoTimestamp);
 FlattenableUtils::write(buffer, size, mFrameNumber);
 FlattenableUtils::write(buffer, size, mBuf);
    writeBoolAsInt(buffer, size, mIsDroppable);
    writeBoolAsInt(buffer, size, mAcquireCalled);
    writeBoolAsInt(buffer, size, mTransformToDisplayInverse);

 return NO_ERROR;
}
