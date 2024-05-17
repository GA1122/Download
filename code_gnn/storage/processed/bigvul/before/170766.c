OMXNodeInstance::~OMXNodeInstance() {
    free(mName);
    CHECK(mHandle == NULL);
}
