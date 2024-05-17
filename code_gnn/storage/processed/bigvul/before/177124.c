static status_t ConvertAvcSpecLevelToOmxAvcLevel(
        WORD32 avcLevel, OMX_VIDEO_AVCLEVELTYPE *omxLevel) {
 for (size_t i = 0; i < NELEM(ConversionTable); ++i) {
 if (avcLevel == ConversionTable[i].avcLevel) {
 *omxLevel = ConversionTable[i].omxLevel;
 return OK;
 }
 }

    ALOGE("ConvertAvcSpecLevelToOmxAvcLevel: %d level not supported",
 (int32_t)avcLevel);

 return BAD_VALUE;
}
