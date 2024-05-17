status_t CameraClient::setPreviewTarget(
 const sp<IGraphicBufferProducer>& bufferProducer) {
    LOG1("setPreviewTarget(%p) (pid %d)", bufferProducer.get(),
            getCallingPid());

    sp<IBinder> binder;
    sp<ANativeWindow> window;
 if (bufferProducer != 0) {
        binder = bufferProducer->asBinder();
        window = new Surface(bufferProducer,   true);
 }
 return setPreviewWindow(binder, window);
}
