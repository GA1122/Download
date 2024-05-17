bool Camera2Client::previewEnabled() {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return false;

 SharedParameters::Lock l(mParameters);
 return l.mParameters.state == Parameters::PREVIEW;
}
