status_t ProCamera2Client::submitRequest(camera_metadata_t* request,
 bool streaming) {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 if (!mExclusiveLock) {
 return PERMISSION_DENIED;
 }

 CameraMetadata metadata(request);

 if (!enforceRequestPermissions(metadata)) {
 return PERMISSION_DENIED;
 }

 if (streaming) {
 return mDevice->setStreamingRequest(metadata);
 } else {
 return mDevice->capture(metadata);
 }

 return OK;
}
