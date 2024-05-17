void CameraSourceListener::postData(int32_t msgType, const sp<IMemory> &dataPtr,
 camera_frame_metadata_t *  ) {
    ALOGV("postData(%d, ptr:%p, size:%zu)",
         msgType, dataPtr->pointer(), dataPtr->size());

    sp<CameraSource> source = mSource.promote();
 if (source.get() != NULL) {
        source->dataCallback(msgType, dataPtr);
 }
}
