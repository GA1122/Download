void CameraClient::handlePreviewData(int32_t msgType,
 const sp<IMemory>& mem,
 camera_frame_metadata_t *metadata) {
 ssize_t offset;
 size_t size;
    sp<IMemoryHeap> heap = mem->getMemory(&offset, &size);

 int flags = mPreviewCallbackFlag;

 if (!(flags & CAMERA_FRAME_CALLBACK_FLAG_ENABLE_MASK)) {
        LOG2("frame callback is disabled");
        mLock.unlock();
 return;
 }

    sp<ICameraClient> c = mRemoteCallback;

 if (c == 0 || (mPreviewCallbackFlag & CAMERA_FRAME_CALLBACK_FLAG_ONE_SHOT_MASK)) {
        LOG2("Disable preview callback");
        mPreviewCallbackFlag &= ~(CAMERA_FRAME_CALLBACK_FLAG_ONE_SHOT_MASK |
                                  CAMERA_FRAME_CALLBACK_FLAG_COPY_OUT_MASK |
                                  CAMERA_FRAME_CALLBACK_FLAG_ENABLE_MASK);
        disableMsgType(CAMERA_MSG_PREVIEW_FRAME);
 }

 if (c != 0) {
 if (flags & CAMERA_FRAME_CALLBACK_FLAG_COPY_OUT_MASK) {
            LOG2("frame is copied");
            copyFrameAndPostCopiedFrame(msgType, c, heap, offset, size, metadata);
 } else {
            LOG2("frame is forwarded");
            mLock.unlock();
            c->dataCallback(msgType, mem, metadata);
 }
 } else {
        mLock.unlock();
 }
}
