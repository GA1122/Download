OggExtractor::OggExtractor(const sp<DataSource> &source)
 : mDataSource(source),
      mInitCheck(NO_INIT),
      mImpl(NULL) {
 for (int i = 0; i < 2; ++i) {
 if (mImpl != NULL) {
 delete mImpl;
 }
 if (i == 0) {
            mImpl = new MyVorbisExtractor(mDataSource);
 } else {
            mImpl = new MyOpusExtractor(mDataSource);
 }
        mInitCheck = mImpl->seekToOffset(0);

 if (mInitCheck == OK) {
            mInitCheck = mImpl->init();
 if (mInitCheck == OK) {
 break;
 }
 }
 }
}
