MPEG4Extractor::MPEG4Extractor(const sp<DataSource> &source)
 : mMoofOffset(0),
      mMoofFound(false),
      mMdatFound(false),
      mDataSource(source),
      mInitCheck(NO_INIT),
      mHeaderTimescale(0),
      mIsQT(false),
      mFirstTrack(NULL),
      mLastTrack(NULL),
      mFileMetaData(new MetaData),
      mFirstSINF(NULL),
      mIsDrm(false) {
}
