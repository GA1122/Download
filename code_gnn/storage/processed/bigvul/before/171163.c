sp<IOMX> MediaPlayerService::getOMX() {
    ALOGI("MediaPlayerService::getOMX");
 Mutex::Autolock autoLock(mLock);

 if (mOMX.get() == NULL) {
        mOMX = new OMX;
 }

 return mOMX;
}
