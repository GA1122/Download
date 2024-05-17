 MediaPlayerService::Client::Client(
         const sp<MediaPlayerService>& service, pid_t pid,
         int32_t connId, const sp<IMediaPlayerClient>& client,
 audio_session_t audioSessionId, uid_t uid)
{
    ALOGV("Client(%d) constructor", connId);
    mPid = pid;
    mConnId = connId;
    mService = service;
    mClient = client;
    mLoop = false;
    mStatus = NO_INIT;
    mAudioSessionId = audioSessionId;
    mUID = uid;
    mRetransmitEndpointValid = false;
    mAudioAttributes = NULL;

#if CALLBACK_ANTAGONIZER
    ALOGD("create Antagonizer");
    mAntagonizer = new Antagonizer(notify, this);
#endif
}
