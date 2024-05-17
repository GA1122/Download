status_t ProCamera2Client::deleteStream(int streamId) {
    ATRACE_CALL();
    ALOGV("%s (streamId = 0x%x)", __FUNCTION__, streamId);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;
    mDevice->clearStreamingRequest();

 status_t code;
 if ((code = mDevice->waitUntilDrained()) != OK) {
        ALOGE("%s: waitUntilDrained failed with code 0x%x", __FUNCTION__, code);
 }

 return mDevice->deleteStream(streamId);
}
