int64_t MediaPlayerService::AudioOutput::getPlayedOutDurationUs(int64_t nowUs) const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0 || mSampleRateHz == 0) {
 return 0;
 }

 uint32_t numFramesPlayed;
 int64_t numFramesPlayedAt;
 AudioTimestamp ts;
 static const int64_t kStaleTimestamp100ms = 100000;

 status_t res = mTrack->getTimestamp(ts);
 if (res == OK) {  
        numFramesPlayed = ts.mPosition;
        numFramesPlayedAt = ts.mTime.tv_sec * 1000000LL + ts.mTime.tv_nsec / 1000;
 const int64_t timestampAge = nowUs - numFramesPlayedAt;
 if (timestampAge > kStaleTimestamp100ms) {
            ALOGV("getTimestamp: returned stale timestamp nowUs(%lld) numFramesPlayedAt(%lld)",
 (long long)nowUs, (long long)numFramesPlayedAt);
            numFramesPlayedAt = nowUs - kStaleTimestamp100ms;
 }
 } else if (res == WOULD_BLOCK) {  
        numFramesPlayed = 0;
        numFramesPlayedAt = nowUs;
 } else {  
        res = mTrack->getPosition(&numFramesPlayed);
        CHECK_EQ(res, (status_t)OK);
        numFramesPlayedAt = nowUs;
        numFramesPlayedAt += 1000LL * mTrack->latency() / 2;  
 }

 int64_t durationUs = (int64_t)((int32_t)numFramesPlayed * 1000000LL / mSampleRateHz)
 + nowUs - numFramesPlayedAt;
 if (durationUs < 0) {
        ALOGV("getPlayedOutDurationUs: negative duration %lld set to zero", (long long)durationUs);
        durationUs = 0;
 }
    ALOGV("getPlayedOutDurationUs(%lld) nowUs(%lld) frames(%u) framesAt(%lld)",
 (long long)durationUs, (long long)nowUs, numFramesPlayed, (long long)numFramesPlayedAt);
 return durationUs;
}
