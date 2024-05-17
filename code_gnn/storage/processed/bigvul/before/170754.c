void OMXNodeInstance::setGraphicBufferSource(
 const sp<GraphicBufferSource>& bufferSource) {
 Mutex::Autolock autoLock(mGraphicBufferSourceLock);
    CLOG_INTERNAL(setGraphicBufferSource, "%p", bufferSource.get());
    mGraphicBufferSource = bufferSource;
}
