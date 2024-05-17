FLACSource::FLACSource(
 const sp<DataSource> &dataSource,
 const sp<MetaData> &trackMetadata)
 : mDataSource(dataSource),
      mTrackMetadata(trackMetadata),
      mParser(0),
      mInitCheck(false),
      mStarted(false)
{
    ALOGV("FLACSource::FLACSource");
    mInitCheck = init();
}
