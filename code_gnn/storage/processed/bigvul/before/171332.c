status_t OMXCodec::findTargetColorFormat(
 const sp<MetaData>& meta, OMX_COLOR_FORMATTYPE *colorFormat) {
    ALOGV("findTargetColorFormat");
    CHECK(mIsEncoder);

 *colorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
 int32_t targetColorFormat;
 if (meta->findInt32(kKeyColorFormat, &targetColorFormat)) {
 *colorFormat = (OMX_COLOR_FORMATTYPE) targetColorFormat;
 }

 return isColorFormatSupported(*colorFormat, kPortIndexInput);
}
