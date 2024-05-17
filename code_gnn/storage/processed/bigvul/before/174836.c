bool CameraService::canConnectUnsafe(int cameraId,
 const String16& clientPackageName,
 const sp<IBinder>& remoteCallback,
                                     sp<BasicClient> &client) {
 String8 clientName8(clientPackageName);
 int callingPid = getCallingPid();

 if (mClient[cameraId] != 0) {
        client = mClient[cameraId].promote();
 if (client != 0) {
 if (remoteCallback == client->getRemote()) {
                LOG1("CameraService::connect X (pid %d) (the same client)",
                     callingPid);
 return true;
 } else {
                ALOGW("CameraService::connect X (pid %d) rejected"
 " (existing client).", callingPid);
 return false;
 }
 }
        mClient[cameraId].clear();
 }

  
 if (mBusy[cameraId]) {
        ALOGW("CameraService::connect X (pid %d, \"%s\") rejected"
 " (camera %d is still busy).", callingPid,
                clientName8.string(), cameraId);
 return false;
 }

 return true;
}
