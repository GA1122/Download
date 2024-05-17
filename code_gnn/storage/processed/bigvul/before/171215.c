status_t MediaPlayerService::Client::setVideoSurfaceTexture(
 const sp<IGraphicBufferProducer>& bufferProducer)
{
    ALOGV("[%d] setVideoSurfaceTexture(%p)", mConnId, bufferProducer.get());
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;

    sp<IBinder> binder(IInterface::asBinder(bufferProducer));
 if (mConnectedWindowBinder == binder) {
 return OK;
 }

    sp<ANativeWindow> anw;
 if (bufferProducer != NULL) {
        anw = new Surface(bufferProducer, true  );
 status_t err = native_window_api_connect(anw.get(),
                NATIVE_WINDOW_API_MEDIA);

 if (err != OK) {
            ALOGE("setVideoSurfaceTexture failed: %d", err);
            reset();

            disconnectNativeWindow();

 return err;
 }
 }

 status_t err = p->setVideoSurfaceTexture(bufferProducer);

    disconnectNativeWindow();

    mConnectedWindow = anw;

 if (err == OK) {
        mConnectedWindowBinder = binder;
 } else {
        disconnectNativeWindow();
 }

 return err;
}
