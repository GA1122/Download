void CameraDeviceClient::notifyShutter(int requestId,
 nsecs_t timestamp) {
    sp<ICameraDeviceCallbacks> remoteCb = getRemoteCallback();
 if (remoteCb != 0) {
        remoteCb->onCaptureStarted(requestId, timestamp);
 }
}
