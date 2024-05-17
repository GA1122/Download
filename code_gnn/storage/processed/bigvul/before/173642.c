DRMSource::~DRMSource() {
 Mutex::Autolock autoLock(mDRMLock);
    mDrmManagerClient->finalizeDecryptUnit(mDecryptHandle, mTrackId);
}
