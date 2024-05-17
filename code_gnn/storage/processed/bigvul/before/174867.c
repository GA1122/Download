void CameraService::Client::OpsCallback::opChanged(int32_t op,
 const String16& packageName) {
    sp<BasicClient> client = mClient.promote();
 if (client != NULL) {
        client->opChanged(op, packageName);
 }
}
