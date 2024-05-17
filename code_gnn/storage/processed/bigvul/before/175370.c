status_t OggSource::start(MetaData *  ) {
 if (mStarted) {
 return INVALID_OPERATION;
 }

    mStarted = true;

 return OK;
}
