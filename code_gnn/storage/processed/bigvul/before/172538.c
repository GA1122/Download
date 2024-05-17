bool StreamingProcessor::haveValidPreviewWindow() const {
 Mutex::Autolock m(mMutex);
 return mPreviewWindow != 0;
}
