void CameraClient::releaseRecordingFrame(const sp<IMemory>& mem) {
 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return;
    mHardware->releaseRecordingFrame(mem);
}
