sp<CameraService::BasicClient> CameraService::findClientUnsafe(
 const wp<IBinder>& cameraClient, int& outIndex) {
    sp<BasicClient> client;

 for (int i = 0; i < mNumberOfCameras; i++) {

 if (mClient[i] == 0) continue;

        client = mClient[i].promote();

 if (client == NULL) {
            mClient[i].clear();
 continue;
 }

 if (cameraClient == client->getRemote()) {
            outIndex = i;
 return client;
 }
 }

    outIndex = -1;
 return NULL;
}
