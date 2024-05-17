sp<DecryptHandle> MediaHTTP::DrmInitialization(const char* mime) {
 if (mDrmManagerClient == NULL) {
        mDrmManagerClient = new DrmManagerClient();
 }

 if (mDrmManagerClient == NULL) {
 return NULL;
 }

 if (mDecryptHandle == NULL) {
        mDecryptHandle = mDrmManagerClient->openDecryptSession(
 String8(mLastURI.c_str()), mime);
 }

 if (mDecryptHandle == NULL) {
 delete mDrmManagerClient;
        mDrmManagerClient = NULL;
 }

 return mDecryptHandle;
}
