void CameraDeviceClient::onFrameAvailable(int32_t requestId,
 const CameraMetadata& frame) {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

    sp<ICameraDeviceCallbacks> remoteCb = mRemoteCallback;
 if (remoteCb != NULL) {
        ALOGV("%s: frame = %p ", __FUNCTION__, &frame);
        remoteCb->onResultReceived(requestId, frame);
 }
}
