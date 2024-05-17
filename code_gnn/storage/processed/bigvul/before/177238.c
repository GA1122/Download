status_t ACodec::initNativeWindow() {
 if (mNativeWindow != NULL) {
 return mOMX->enableGraphicBuffers(mNode, kPortIndexOutput, OMX_TRUE);
 }

    mOMX->enableGraphicBuffers(mNode, kPortIndexOutput, OMX_FALSE);
 return OK;
}
