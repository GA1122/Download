status_t StreamingProcessor::setPreviewWindow(sp<Surface> window) {
    ATRACE_CALL();
 status_t res;

    res = deletePreviewStream();
 if (res != OK) return res;

 Mutex::Autolock m(mMutex);

    mPreviewWindow = window;

 return OK;
}
