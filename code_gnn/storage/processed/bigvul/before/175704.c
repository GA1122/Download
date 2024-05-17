status_t FLACSource::start(MetaData *  )
{
    ALOGV("FLACSource::start");

    CHECK(!mStarted);
    mParser->allocateBuffers();
    mStarted = true;

 return OK;
}
