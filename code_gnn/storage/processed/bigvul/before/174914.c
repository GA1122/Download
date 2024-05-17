status_t Camera2Client::setParameters(const String8& params) {
    ATRACE_CALL();
    ALOGV("%s: Camera %d", __FUNCTION__, mCameraId);
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 SharedParameters::Lock l(mParameters);

    res = l.mParameters.set(params);
 if (res != OK) return res;

    res = updateRequests(l.mParameters);

 return res;
}
