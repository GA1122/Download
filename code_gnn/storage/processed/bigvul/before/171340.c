static size_t getFrameSize(
        OMX_COLOR_FORMATTYPE colorFormat, int32_t width, int32_t height) {
 switch (colorFormat) {
 case OMX_COLOR_FormatYCbYCr:
 case OMX_COLOR_FormatCbYCrY:
 return width * height * 2;

 case OMX_COLOR_FormatYUV420Planar:
 case OMX_COLOR_FormatYUV420SemiPlanar:
 case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar:
  
 case OMX_COLOR_FormatAndroidOpaque:
 return (width * height * 3) / 2;

 default:
            CHECK(!"Should not be here. Unsupported color format.");
 break;
 }
 return 0;
}
