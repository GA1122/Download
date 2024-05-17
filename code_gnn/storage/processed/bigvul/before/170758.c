status_t OMXNodeInstance::signalEndOfInputStream() {
    sp<GraphicBufferSource> bufferSource(getGraphicBufferSource());
 if (bufferSource == NULL) {
        CLOGW("signalEndOfInputStream can only be used with Surface input");
 return INVALID_OPERATION;
 }
 return bufferSource->signalEndOfInputStream();
}
