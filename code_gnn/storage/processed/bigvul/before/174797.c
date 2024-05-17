size_t IGraphicBufferConsumer::BufferItem::getPodSize() const {
 size_t c = sizeof(mCrop) +
 sizeof(mTransform) +
 sizeof(mScalingMode) +
 sizeof(mTimestamp) +
 sizeof(mIsAutoTimestamp) +
 sizeof(mFrameNumber) +
 sizeof(mBuf) +
 sizeof(mIsDroppable) +
 sizeof(mAcquireCalled) +
 sizeof(mTransformToDisplayInverse);
 return c;
}
