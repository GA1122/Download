void CameraDeviceClient::notifyError() {
    sp<ICameraDeviceCallbacks> remoteCb = getRemoteCallback();

 if (remoteCb != 0) {
        remoteCb->onDeviceError(ICameraDeviceCallbacks::ERROR_CAMERA_DEVICE);
 }
}
