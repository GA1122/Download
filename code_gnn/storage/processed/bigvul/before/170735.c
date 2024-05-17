sp<GraphicBufferSource> OMXNodeInstance::getGraphicBufferSource() {
 Mutex::Autolock autoLock(mGraphicBufferSourceLock);
 return mGraphicBufferSource;
}
