void ProCamera2Client::onFrameAvailable(int32_t requestId,
 const CameraMetadata& frame) {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 Mutex::Autolock icl(mBinderSerializationLock);
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);

 if (mRemoteCallback != NULL) {
 CameraMetadata tmp(frame);
 camera_metadata_t* meta = tmp.release();
        ALOGV("%s: meta = %p ", __FUNCTION__, meta);
        mRemoteCallback->onResultReceived(requestId, meta);
        tmp.acquire(meta);
 }

}
