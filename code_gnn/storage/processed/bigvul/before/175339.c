MyOggExtractor::MyOggExtractor(
 const sp<DataSource> &source,
 const char *mimeType,
 size_t numHeaders,
 int64_t seekPreRollUs)
 : mSource(source),
      mOffset(0),
      mCurGranulePosition(0),
      mPrevGranulePosition(0),
      mCurrentPageSize(0),
      mFirstPacketInPage(true),
      mCurrentPageSamples(0),
      mNextLaceIndex(0),
      mMimeType(mimeType),
      mNumHeaders(numHeaders),
      mSeekPreRollUs(seekPreRollUs),
      mFirstDataOffset(-1) {
    mCurrentPage.mNumSegments = 0;

    vorbis_info_init(&mVi);
    vorbis_comment_init(&mVc);
}
