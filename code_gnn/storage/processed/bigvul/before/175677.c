void FLACParser::errorCallback(FLAC__StreamDecoderErrorStatus status)
{
    ALOGE("FLACParser::errorCallback status=%d", status);
    mErrorStatus = status;
}
