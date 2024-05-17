sp<IMediaRecorder> MediaPlayerService::createMediaRecorder(const String16 &opPackageName)
{
 pid_t pid = IPCThreadState::self()->getCallingPid();
    sp<MediaRecorderClient> recorder = new MediaRecorderClient(this, pid, opPackageName);
    wp<MediaRecorderClient> w = recorder;
 Mutex::Autolock lock(mLock);
    mMediaRecorderClients.add(w);
    ALOGV("Create new media recorder client from pid %d", pid);
 return recorder;
}
