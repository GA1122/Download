void CameraClient::copyFrameAndPostCopiedFrame(
 int32_t msgType, const sp<ICameraClient>& client,
 const sp<IMemoryHeap>& heap, size_t offset, size_t size,
 camera_frame_metadata_t *metadata) {
    LOG2("copyFrameAndPostCopiedFrame");
    sp<MemoryHeapBase> previewBuffer;

 if (mPreviewBuffer == 0) {
        mPreviewBuffer = new MemoryHeapBase(size, 0, NULL);
 } else if (size > mPreviewBuffer->virtualSize()) {
        mPreviewBuffer.clear();
        mPreviewBuffer = new MemoryHeapBase(size, 0, NULL);
 }
 if (mPreviewBuffer == 0) {
        ALOGE("failed to allocate space for preview buffer");
        mLock.unlock();
 return;
 }
    previewBuffer = mPreviewBuffer;

    memcpy(previewBuffer->base(), (uint8_t *)heap->base() + offset, size);

    sp<MemoryBase> frame = new MemoryBase(previewBuffer, 0, size);
 if (frame == 0) {
        ALOGE("failed to allocate space for frame callback");
        mLock.unlock();
 return;
 }

    mLock.unlock();
    client->dataCallback(msgType, frame, metadata);
}
