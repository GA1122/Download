MPEG4Extractor::MPEG4Extractor(const sp<DataSource> &source)
 : mSidxDuration(0),
      mMoofOffset(0),
      mDataSource(source),
      mInitCheck(NO_INIT),
      mHasVideo(false),
      mHeaderTimescale(0),
      mFirstTrack(NULL),
      mLastTrack(NULL),
      mFileMetaData(new MetaData),
      mFirstSINF(NULL),
      mIsDrm(false) {
}
