status_t ProCamera2Client::createStream(int width, int height, int format,
 const sp<IGraphicBufferProducer>& bufferProducer,
  
 int* streamId)
{
 if (streamId) {
 *streamId = -1;
 }

    ATRACE_CALL();
    ALOGV("%s (w = %d, h = %d, f = 0x%x)", __FUNCTION__, width, height, format);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

    sp<IBinder> binder;
    sp<ANativeWindow> window;
 if (bufferProducer != 0) {
        binder = bufferProducer->asBinder();
        window = new Surface(bufferProducer);
 }

 return mDevice->createStream(window, width, height, format,  1,
                                 streamId);
}
