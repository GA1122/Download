AMRSource::AMRSource(
 const sp<DataSource> &source, const sp<MetaData> &meta,
 bool isWide, const off64_t *offset_table, size_t offset_table_length)
 : mDataSource(source),
      mMeta(meta),
      mIsWide(isWide),
      mOffset(mIsWide ? 9 : 6),
      mCurrentTimeUs(0),
      mStarted(false),
      mGroup(NULL),
      mOffsetTableLength(offset_table_length) {
 if (mOffsetTableLength > 0 && mOffsetTableLength <= OFFSET_TABLE_LEN) {
        memcpy ((char*)mOffsetTable, (char*)offset_table, sizeof(off64_t) * mOffsetTableLength);
 }
}
