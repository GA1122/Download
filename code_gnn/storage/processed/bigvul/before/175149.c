NuPlayer::GenericSource::GenericSource(
 const sp<AMessage> &notify,
 bool uidValid,
 uid_t uid)
 : Source(notify),
      mFetchSubtitleDataGeneration(0),
      mFetchTimedTextDataGeneration(0),
      mDurationUs(0ll),
      mAudioIsVorbis(false),
      mIsWidevine(false),
      mUIDValid(uidValid),
      mUID(uid),
      mDrmManagerClient(NULL),
      mMetaDataSize(-1ll),
      mBitrate(-1ll),
      mPollBufferingGeneration(0),
      mPendingReadBufferTypes(0) {
    resetDataSource();
 DataSource::RegisterDefaultSniffers();
}
