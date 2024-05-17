status_t CameraService::connectFinishUnsafe(const sp<BasicClient>& client,
 const sp<IBinder>& remoteCallback) {
 status_t status = client->initialize(mModule);
 if (status != OK) {
 return status;
 }

    remoteCallback->linkToDeath(this);

 return OK;
}
