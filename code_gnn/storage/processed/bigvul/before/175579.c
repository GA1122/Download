sp<IMediaSource> AMRExtractor::getTrack(size_t index) {
 if (mInitCheck != OK || index != 0) {
 return NULL;
 }

 return new AMRSource(mDataSource, mMeta, mIsWide,
            mOffsetTable, mOffsetTableLength);
}
