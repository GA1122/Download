status_t CameraDeviceClient::deleteStream(int streamId) {
    ATRACE_CALL();
    ALOGV("%s (streamId = 0x%x)", __FUNCTION__, streamId);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 ssize_t index = NAME_NOT_FOUND;
 for (size_t i = 0; i < mStreamMap.size(); ++i) {
 if (streamId == mStreamMap.valueAt(i)) {
            index = i;
 break;
 }
 }

 if (index == NAME_NOT_FOUND) {
        ALOGW("%s: Camera %d: Invalid stream ID (%d) specified, no stream "
 "created yet", __FUNCTION__, mCameraId, streamId);
 return BAD_VALUE;
 }

    res = mDevice->deleteStream(streamId);

 if (res == BAD_VALUE) {
        ALOGE("%s: Camera %d: Unexpected BAD_VALUE when deleting stream, but we"
 " already checked and the stream ID (%d) should be valid.",
              __FUNCTION__, mCameraId, streamId);
 } else if (res == OK) {
        mStreamMap.removeItemsAt(index);

        ALOGV("%s: Camera %d: Successfully deleted stream ID (%d)",
              __FUNCTION__, mCameraId, streamId);
 }

 return res;
}
