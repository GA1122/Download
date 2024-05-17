status_t CameraDeviceClient::waitUntilIdle()
{
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 status_t res = OK;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 if (!mStreamingRequestList.isEmpty()) {
        ALOGE("%s: Camera %d: Try to waitUntilIdle when there are active streaming requests",
              __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 }
    res = mDevice->waitUntilDrained();
    ALOGV("%s Done", __FUNCTION__);

 return res;
}
