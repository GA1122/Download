status_t AMRSource::stop() {
    CHECK(mStarted);

 delete mGroup;
    mGroup = NULL;

    mStarted = false;
 return OK;
}
