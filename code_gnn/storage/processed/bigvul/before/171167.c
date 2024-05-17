status_t MediaPlayerService::Client::getPlaybackSettings(AudioPlaybackRate* rate  )
{
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 status_t ret = p->getPlaybackSettings(rate);
 if (ret == NO_ERROR) {
        ALOGV("[%d] getPlaybackSettings(%f, %f, %d, %d)",
                mConnId, rate->mSpeed, rate->mPitch, rate->mFallbackMode, rate->mStretchMode);
 } else {
        ALOGV("[%d] getPlaybackSettings returned %d", mConnId, ret);
 }
 return ret;
}
