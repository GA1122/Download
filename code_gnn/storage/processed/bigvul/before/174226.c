Camera3Device::Size Camera3Device::getMaxJpegResolution() const {
 int32_t maxJpegWidth = 0, maxJpegHeight = 0;
 if (mDeviceVersion >= CAMERA_DEVICE_API_VERSION_3_2) {
 const int STREAM_CONFIGURATION_SIZE = 4;
 const int STREAM_FORMAT_OFFSET = 0;
 const int STREAM_WIDTH_OFFSET = 1;
 const int STREAM_HEIGHT_OFFSET = 2;
 const int STREAM_IS_INPUT_OFFSET = 3;
 camera_metadata_ro_entry_t availableStreamConfigs =
                mDeviceInfo.find(ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS);
 if (availableStreamConfigs.count == 0 ||
                availableStreamConfigs.count % STREAM_CONFIGURATION_SIZE != 0) {
 return Size(0, 0);
 }

 for (size_t i=0; i < availableStreamConfigs.count; i+= STREAM_CONFIGURATION_SIZE) {
 int32_t format = availableStreamConfigs.data.i32[i + STREAM_FORMAT_OFFSET];
 int32_t width = availableStreamConfigs.data.i32[i + STREAM_WIDTH_OFFSET];
 int32_t height = availableStreamConfigs.data.i32[i + STREAM_HEIGHT_OFFSET];
 int32_t isInput = availableStreamConfigs.data.i32[i + STREAM_IS_INPUT_OFFSET];
 if (isInput == ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT
 && format == HAL_PIXEL_FORMAT_BLOB &&
 (width * height > maxJpegWidth * maxJpegHeight)) {
                maxJpegWidth = width;
                maxJpegHeight = height;
 }
 }
 } else {
        camera_metadata_ro_entry availableJpegSizes =
                mDeviceInfo.find(ANDROID_SCALER_AVAILABLE_JPEG_SIZES);
 if (availableJpegSizes.count == 0 || availableJpegSizes.count % 2 != 0) {
 return Size(0, 0);
 }

 for (size_t i = 0; i < availableJpegSizes.count; i += 2) {
 if ((availableJpegSizes.data.i32[i] * availableJpegSizes.data.i32[i + 1])
 > (maxJpegWidth * maxJpegHeight)) {
                maxJpegWidth = availableJpegSizes.data.i32[i];
                maxJpegHeight = availableJpegSizes.data.i32[i + 1];
 }
 }
 }
 return Size(maxJpegWidth, maxJpegHeight);
}
