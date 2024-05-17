ssize_t Camera3Device::getJpegBufferSize(uint32_t width, uint32_t height) const {
 Size maxJpegResolution = getMaxJpegResolution();
 if (maxJpegResolution.width == 0) {
        ALOGE("%s: Camera %d: Can't find valid available jpeg sizes in static metadata!",
                __FUNCTION__, mId);
 return BAD_VALUE;
 }

 ssize_t maxJpegBufferSize = 0;
    camera_metadata_ro_entry jpegBufMaxSize = mDeviceInfo.find(ANDROID_JPEG_MAX_SIZE);
 if (jpegBufMaxSize.count == 0) {
        ALOGE("%s: Camera %d: Can't find maximum JPEG size in static metadata!", __FUNCTION__, mId);
 return BAD_VALUE;
 }
    maxJpegBufferSize = jpegBufMaxSize.data.i32[0];
    assert(kMinJpegBufferSize < maxJpegBufferSize);

 float scaleFactor = ((float) (width * height)) /
 (maxJpegResolution.width * maxJpegResolution.height);
 ssize_t jpegBufferSize = scaleFactor * (maxJpegBufferSize - kMinJpegBufferSize) +
            kMinJpegBufferSize;
 if (jpegBufferSize > maxJpegBufferSize) {
        jpegBufferSize = maxJpegBufferSize;
 }

 return jpegBufferSize;
}
