sp<IMediaPlayer> MediaPlayerService::create(const sp<IMediaPlayerClient>& client,
 audio_session_t audioSessionId)
{
 pid_t pid = IPCThreadState::self()->getCallingPid();
 int32_t connId = android_atomic_inc(&mNextConnId);

    sp<Client> c = new Client(
 this, pid, connId, client, audioSessionId,
 IPCThreadState::self()->getCallingUid());

    ALOGV("Create new client(%d) from pid %d, uid %d, ", connId, pid,
 IPCThreadState::self()->getCallingUid());

    wp<Client> w = c;
 {
 Mutex::Autolock lock(mLock);
        mClients.add(w);
 }
 return c;
}
