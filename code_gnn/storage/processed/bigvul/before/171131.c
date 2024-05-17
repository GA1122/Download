MediaPlayerService::MediaPlayerService()
{
    ALOGV("MediaPlayerService created");
    mNextConnId = 1;

    mBatteryAudio.refCount = 0;
 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
        mBatteryAudio.deviceOn[i] = 0;
        mBatteryAudio.lastTime[i] = 0;
        mBatteryAudio.totalTime[i] = 0;
 }
    mBatteryAudio.deviceOn[SPEAKER] = 1;

 BatteryNotifier::getInstance().noteResetVideo();

 MediaPlayerFactory::registerBuiltinFactories();
}
