NuPlayer::GenericSource::GenericSource(
 const sp<AMessage> &notify,
 bool uidValid,
 uid_t uid)
 : Source(notify),
      mAudioTimeUs(0),
      mAudioLastDequeueTimeUs(0),
      mVideoTimeUs(0),
      mVideoLastDequeueTimeUs(0),
      mFetchSubtitleDataGeneration(0),
      mFetchTimedTextDataGeneration(0),
      mDurationUs(-1ll),
      mAudioIsVorbis(false),
      mIsWidevine(false),
      mIsSecure(false),
      mIsStreaming(false),
      mUIDValid(uidValid),
      mUID(uid),
      mFd(-1),
      mDrmManagerClient(NULL),
      mBitrate(-1ll),
      mPollBufferingGeneration(0),
      mPendingReadBufferTypes(0),
      mBuffering(false),
      mPrepareBuffering(false),
      mPrevBufferPercentage(-1) {
    resetDataSource();
 DataSource::RegisterDefaultSniffers();
}
