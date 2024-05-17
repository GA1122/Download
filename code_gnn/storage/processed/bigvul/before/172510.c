static int32_t getColorFormat(const char* colorFormat) {
 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_YUV420P)) {
 return OMX_COLOR_FormatYUV420Planar;
 }

 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_YUV422SP)) {
 return OMX_COLOR_FormatYUV422SemiPlanar;
 }

 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_YUV420SP)) {
 return OMX_COLOR_FormatYUV420SemiPlanar;
 }

 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_YUV422I)) {
 return OMX_COLOR_FormatYCbYCr;
 }

 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_RGB565)) {
 return OMX_COLOR_Format16bitRGB565;
 }

 if (!strcmp(colorFormat, "OMX_TI_COLOR_FormatYUV420PackedSemiPlanar")) {
 return OMX_TI_COLOR_FormatYUV420PackedSemiPlanar;
 }

 if (!strcmp(colorFormat, CameraParameters::PIXEL_FORMAT_ANDROID_OPAQUE)) {
 return OMX_COLOR_FormatAndroidOpaque;
 }

    ALOGE("Uknown color format (%s), please add it to "
 "CameraSource::getColorFormat", colorFormat);

    CHECK(!"Unknown color format");
 return -1;
}
