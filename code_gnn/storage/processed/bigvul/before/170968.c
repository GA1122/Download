void MediaHTTP::getDrmInfo(
        sp<DecryptHandle> &handle, DrmManagerClient **client) {
    handle = mDecryptHandle;
 *client = mDrmManagerClient;
}
