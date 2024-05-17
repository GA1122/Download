void CameraClient::handleRawPicture(const sp<IMemory>& mem) {
    disableMsgType(CAMERA_MSG_RAW_IMAGE);

 ssize_t offset;
 size_t size;
    sp<IMemoryHeap> heap = mem->getMemory(&offset, &size);

    sp<ICameraClient> c = mRemoteCallback;
    mLock.unlock();
 if (c != 0) {
        c->dataCallback(CAMERA_MSG_RAW_IMAGE, mem, NULL);
 }
}
