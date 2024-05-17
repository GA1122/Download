status_t CameraDeviceClient::createStream(int width, int height, int format,
 const sp<IGraphicBufferProducer>& bufferProducer)
{
    ATRACE_CALL();
    ALOGV("%s (w = %d, h = %d, f = 0x%x)", __FUNCTION__, width, height, format);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 {
 ssize_t index = mStreamMap.indexOfKey(bufferProducer->asBinder());
 if (index != NAME_NOT_FOUND) {
            ALOGW("%s: Camera %d: Buffer producer already has a stream for it "
 "(ID %d)",
                  __FUNCTION__, mCameraId, index);
 return ALREADY_EXISTS;
 }
 }

 bool useAsync = false;
 int32_t consumerUsage;
 if ((res = bufferProducer->query(NATIVE_WINDOW_CONSUMER_USAGE_BITS,
 &consumerUsage)) != OK) {
        ALOGE("%s: Camera %d: Failed to query consumer usage", __FUNCTION__,
              mCameraId);
 return res;
 }
 if (consumerUsage & GraphicBuffer::USAGE_HW_TEXTURE) {
        ALOGW("%s: Camera %d: Forcing asynchronous mode for stream",
                __FUNCTION__, mCameraId);
        useAsync = true;
 }

    sp<IBinder> binder;
    sp<ANativeWindow> anw;
 if (bufferProducer != 0) {
        binder = bufferProducer->asBinder();
        anw = new Surface(bufferProducer, useAsync);
 }


 if ((res = anw->query(anw.get(), NATIVE_WINDOW_WIDTH, &width)) != OK) {
        ALOGE("%s: Camera %d: Failed to query Surface width", __FUNCTION__,
              mCameraId);
 return res;
 }
 if ((res = anw->query(anw.get(), NATIVE_WINDOW_HEIGHT, &height)) != OK) {
        ALOGE("%s: Camera %d: Failed to query Surface height", __FUNCTION__,
              mCameraId);
 return res;
 }
 if ((res = anw->query(anw.get(), NATIVE_WINDOW_FORMAT, &format)) != OK) {
        ALOGE("%s: Camera %d: Failed to query Surface format", __FUNCTION__,
              mCameraId);
 return res;
 }

 if (format >= HAL_PIXEL_FORMAT_RGBA_8888 &&
        format <= HAL_PIXEL_FORMAT_BGRA_8888) {
        ALOGW("%s: Camera %d: Overriding format 0x%x to IMPLEMENTATION_DEFINED",
              __FUNCTION__, mCameraId, format);
        format = HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED;
 }


 int streamId = -1;
 if (format == HAL_PIXEL_FORMAT_BLOB) {
 CameraMetadata staticInfo = mDevice->info();
 camera_metadata_entry_t entry = staticInfo.find(ANDROID_JPEG_MAX_SIZE);
 if (entry.count == 0) {
            ALOGE("%s: Camera %d: Can't find maximum JPEG size in "
 "static metadata!", __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 }
 int32_t maxJpegSize = entry.data.i32[0];
        res = mDevice->createStream(anw, width, height, format, maxJpegSize,
 &streamId);
 } else {
        res = mDevice->createStream(anw, width, height, format,  0,
 &streamId);
 }

 if (res == OK) {
        mStreamMap.add(bufferProducer->asBinder(), streamId);

        ALOGV("%s: Camera %d: Successfully created a new stream ID %d",
              __FUNCTION__, mCameraId, streamId);

  
 int32_t transform = 0;
        res = getRotationTransformLocked(&transform);

 if (res != OK) {
 return res;
 }

        res = mDevice->setStreamTransform(streamId, transform);
 if (res != OK) {
            ALOGE("%s: Failed to set stream transform (stream id %d)",
                  __FUNCTION__, streamId);
 return res;
 }

 return streamId;
 }

 return res;
}
