status_t ACodec::setupVideoDecoder(
 const char *mime, const sp<AMessage> &msg, bool haveNativeWindow) {
 int32_t width, height;
 if (!msg->findInt32("width", &width)
 || !msg->findInt32("height", &height)) {
 return INVALID_OPERATION;
 }

    OMX_VIDEO_CODINGTYPE compressionFormat;
 status_t err = GetVideoCodingTypeFromMime(mime, &compressionFormat);

 if (err != OK) {
 return err;
 }

    err = setVideoPortFormatType(
            kPortIndexInput, compressionFormat, OMX_COLOR_FormatUnused);

 if (err != OK) {
 return err;
 }

 int32_t tmp;
 if (msg->findInt32("color-format", &tmp)) {
        OMX_COLOR_FORMATTYPE colorFormat =
 static_cast<OMX_COLOR_FORMATTYPE>(tmp);
        err = setVideoPortFormatType(
                kPortIndexOutput, OMX_VIDEO_CodingUnused, colorFormat, haveNativeWindow);
 if (err != OK) {
            ALOGW("[%s] does not support color format %d",
                  mComponentName.c_str(), colorFormat);
            err = setSupportedOutputFormat(!haveNativeWindow  );
 }
 } else {
        err = setSupportedOutputFormat(!haveNativeWindow  );
 }

 if (err != OK) {
 return err;
 }

 int32_t frameRateInt;
 float frameRateFloat;
 if (!msg->findFloat("frame-rate", &frameRateFloat)) {
 if (!msg->findInt32("frame-rate", &frameRateInt)) {
            frameRateInt = -1;
 }
        frameRateFloat = (float)frameRateInt;
 }

    err = setVideoFormatOnPort(
            kPortIndexInput, width, height, compressionFormat, frameRateFloat);

 if (err != OK) {
 return err;
 }

    err = setVideoFormatOnPort(
            kPortIndexOutput, width, height, OMX_VIDEO_CodingUnused);

 if (err != OK) {
 return err;
 }

 return OK;
}
