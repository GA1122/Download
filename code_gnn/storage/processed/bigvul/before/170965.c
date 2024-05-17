void MediaHTTP::clearDRMState_l() {
 if (mDecryptHandle != NULL) {
        CHECK(mDrmManagerClient);
        mDrmManagerClient->closeDecryptSession(mDecryptHandle);
        mDecryptHandle = NULL;
 }
}
