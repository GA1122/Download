SoftMPEG4::~SoftMPEG4() {
 if (mInitialized) {
 PVCleanUpVideoDecoder(mHandle);
 }

 delete mHandle;
    mHandle = NULL;
}
