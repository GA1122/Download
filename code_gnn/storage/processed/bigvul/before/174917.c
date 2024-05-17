status_t Camera2Client::setPreviewTarget(
 const sp<IGraphicBufferProducer>& bufferProducer) {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

    sp<IBinder> binder;
    sp<ANativeWindow> window;
 if (bufferProducer != 0) {
        binder = bufferProducer->asBinder();
        window = new Surface(bufferProducer,   true);
 }
 return setPreviewWindowL(binder, window);
}
