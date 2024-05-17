status_t OMXCodec::initNativeWindow() {
 status_t err = mOMX->enableGraphicBuffers(mNode, kPortIndexOutput, OMX_TRUE);
 if (err != 0) {
 return err;
 }

 return OK;
}
