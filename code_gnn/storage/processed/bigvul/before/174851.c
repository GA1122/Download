sp<CameraService::BasicClient> CameraService::getClientByRemote(
 const wp<IBinder>& cameraClient) {

    sp<BasicClient> client;

 Mutex::Autolock lock(mServiceLock);

 int outIndex;
    client = findClientUnsafe(cameraClient, outIndex);

 return client;
}
