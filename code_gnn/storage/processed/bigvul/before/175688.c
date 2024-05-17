status_t FLACSource::init()
{
    ALOGV("FLACSource::init");
    mParser = new FLACParser(mDataSource);
 return mParser->initCheck();
}
