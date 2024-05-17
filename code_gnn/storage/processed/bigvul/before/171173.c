status_t MediaPlayerService::Client::getSyncSettings(
 AVSyncSettings* sync  , float* videoFps  )
{
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 status_t ret = p->getSyncSettings(sync, videoFps);
 if (ret == NO_ERROR) {
        ALOGV("[%d] getSyncSettings(%u, %u, %f, %f)",
                mConnId, sync->mSource, sync->mAudioAdjustMode, sync->mTolerance, *videoFps);
 } else {
        ALOGV("[%d] getSyncSettings returned %d", mConnId, ret);
 }
 return ret;
}
