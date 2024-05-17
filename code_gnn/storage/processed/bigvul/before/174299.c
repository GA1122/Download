NuPlayer::NuPlayerStreamListener::NuPlayerStreamListener(
 const sp<IStreamSource> &source,
 const sp<AHandler> &targetHandler)
 : mSource(source),
      mTargetHandler(targetHandler),
      mEOS(false),
      mSendDataNotification(true) {
    mSource->setListener(this);

    mMemoryDealer = new MemoryDealer(kNumBuffers * kBufferSize);
 for (size_t i = 0; i < kNumBuffers; ++i) {
        sp<IMemory> mem = mMemoryDealer->allocate(kBufferSize);
        CHECK(mem != NULL);

        mBuffers.push(mem);
 }
    mSource->setBuffers(mBuffers);
}
