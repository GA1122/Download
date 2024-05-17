MediaPlayer::~MediaPlayer()
{
    ALOGV("destructor");
 if (mAudioAttributesParcel != NULL) {
 delete mAudioAttributesParcel;
        mAudioAttributesParcel = NULL;
 }
 AudioSystem::releaseAudioSessionId(mAudioSessionId, -1);
    disconnect();
 IPCThreadState::self()->flushCommands();
}
