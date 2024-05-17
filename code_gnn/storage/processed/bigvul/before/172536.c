int StreamingProcessor::getPreviewStreamId() const {
 Mutex::Autolock m(mMutex);
 return mPreviewStreamId;
}
