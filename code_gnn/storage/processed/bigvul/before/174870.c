void CameraService::removeClientByRemote(const wp<IBinder>& remoteBinder) {
 int callingPid = getCallingPid();
    LOG1("CameraService::removeClientByRemote E (pid %d)", callingPid);

 Mutex::Autolock lock(mServiceLock);

 int outIndex;
    sp<BasicClient> client = findClientUnsafe(remoteBinder, outIndex);

 if (client != 0) {
        LOG1("removeClient: clear camera %d", outIndex);
        mClient[outIndex].clear();

        client->getRemote()->unlinkToDeath(this);
 } else {

        sp<ProClient> clientPro = findProClientUnsafe(remoteBinder);

 if (clientPro != NULL) {
            LOG1("removeClient: clear pro %p", clientPro.get());

            clientPro->getRemoteCallback()->asBinder()->unlinkToDeath(this);
 }
 }

    LOG1("CameraService::removeClientByRemote X (pid %d)", callingPid);
}
