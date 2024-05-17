status_t OMXNodeInstance::createInputSurface(
        OMX_U32 portIndex, sp<IGraphicBufferProducer> *bufferProducer, MetadataBufferType *type) {
 Mutex::Autolock autolock(mLock);
 status_t err = createGraphicBufferSource(portIndex, NULL  , type);

 if (err != OK) {
 return err;
 }

 *bufferProducer = mGraphicBufferSource->getIGraphicBufferProducer();
 return OK;
}
