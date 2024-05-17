status_t OMXNodeInstance::setInputSurface(
        OMX_U32 portIndex, const sp<IGraphicBufferConsumer> &bufferConsumer,
 MetadataBufferType *type) {
 Mutex::Autolock autolock(mLock);
 return createGraphicBufferSource(portIndex, bufferConsumer, type);
}
