void ACodec::signalError(OMX_ERRORTYPE error, status_t internalError) {
    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", CodecBase::kWhatError);
    ALOGE("signalError(omxError %#x, internalError %d)", error, internalError);

 if (internalError == UNKNOWN_ERROR) {  
 const status_t omxStatus = statusFromOMXError(error);
 if (omxStatus != 0) {
            internalError = omxStatus;
 } else {
            ALOGW("Invalid OMX error %#x", error);
 }
 }

    mFatalError = true;

    notify->setInt32("err", internalError);
    notify->setInt32("actionCode", ACTION_CODE_FATAL);  
    notify->post();
}
