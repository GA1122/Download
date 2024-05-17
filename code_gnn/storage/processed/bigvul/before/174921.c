status_t Camera2Client::startRecording() {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;
 SharedParameters::Lock l(mParameters);

 return startRecordingL(l.mParameters, false);
}
