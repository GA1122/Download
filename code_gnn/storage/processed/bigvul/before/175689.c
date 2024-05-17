status_t FLACExtractor::init()
{
    mFileMetadata = new MetaData;
    mTrackMetadata = new MetaData;
    mParser = new FLACParser(mDataSource, mFileMetadata, mTrackMetadata);
 return mParser->initCheck();
}
