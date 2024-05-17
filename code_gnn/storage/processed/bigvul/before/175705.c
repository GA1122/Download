status_t FLACSource::stop()
{
    ALOGV("FLACSource::stop");

    CHECK(mStarted);
    mParser->releaseBuffers();
    mStarted = false;

 return OK;
}
