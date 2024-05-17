void Camera2Client::releaseRecordingFrame(const sp<IMemory>& mem) {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
 if ( checkPid(__FUNCTION__) != OK) return;

    mStreamingProcessor->releaseRecordingFrame(mem);
}
