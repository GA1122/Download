status_t Camera3Device::getInputBufferProducer(
        sp<IGraphicBufferProducer> *producer) {
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 if (producer == NULL) {
 return BAD_VALUE;
 } else if (mInputStream == NULL) {
 return INVALID_OPERATION;
 }

 return mInputStream->getInputBufferProducer(producer);
}
