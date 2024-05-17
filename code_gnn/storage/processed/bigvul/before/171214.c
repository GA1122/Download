status_t MediaPlayerService::Client::setSyncSettings(
 const AVSyncSettings& sync, float videoFpsHint)
{
    ALOGV("[%d] setSyncSettings(%u, %u, %f, %f)",
            mConnId, sync.mSource, sync.mAudioAdjustMode, sync.mTolerance, videoFpsHint);
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 return p->setSyncSettings(sync, videoFpsHint);
}
