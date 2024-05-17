status_t MediaPlayer::setSyncSettings(const AVSyncSettings& sync, float videoFpsHint)
{
    ALOGV("setSyncSettings: %u %u %f %f",
            sync.mSource, sync.mAudioAdjustMode, sync.mTolerance, videoFpsHint);
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 return mPlayer->setSyncSettings(sync, videoFpsHint);
}
