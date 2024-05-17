sp<CameraService::ProClient> CameraService::findProClientUnsafe(
 const wp<IBinder>& cameraCallbacksRemote)
{
    sp<ProClient> clientPro;

 for (int i = 0; i < mNumberOfCameras; ++i) {
 Vector<size_t> removeIdx;

 for (size_t j = 0; j < mProClientList[i].size(); ++j) {
            wp<ProClient> cl = mProClientList[i][j];

            sp<ProClient> clStrong = cl.promote();
 if (clStrong != NULL && clStrong->getRemote() == cameraCallbacksRemote) {
                clientPro = clStrong;
 break;
 } else if (clStrong == NULL) {
                removeIdx.push(j);
 }
 }

 for (ssize_t j = (ssize_t)removeIdx.size() - 1; j >= 0; --j) {
            mProClientList[i].removeAt(removeIdx[j]);
 }

 }

 return clientPro;
}
