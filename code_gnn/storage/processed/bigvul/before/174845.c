status_t CameraService::BasicClient::finishCameraOps() {
 if (mOpsActive) {
        mAppOpsManager.finishOp(AppOpsManager::OP_CAMERA, mClientUid,
                mClientPackageName);
        mOpsActive = false;
 }
    mAppOpsManager.stopWatchingMode(mOpsCallback);
    mOpsCallback.clear();

 return OK;
}
