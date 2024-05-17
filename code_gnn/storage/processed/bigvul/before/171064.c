ID3::ID3(const sp<DataSource> &source, bool ignoreV1, off64_t offset)
 : mIsValid(false),
      mData(NULL),
      mSize(0),
      mFirstFrameOffset(0),
      mVersion(ID3_UNKNOWN),
      mRawSize(0) {
    mIsValid = parseV2(source, offset);

 if (!mIsValid && !ignoreV1) {
        mIsValid = parseV1(source);
 }
}
