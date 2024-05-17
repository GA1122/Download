static status_t ConvertAvcSpecLevelToOmxAvcLevel(
 AVCLevel avcLevel, OMX_U32 *omxLevel) {
 for (size_t i = 0, n = sizeof(ConversionTable)/sizeof(ConversionTable[0]);
        i < n; ++i) {
 if (avcLevel == ConversionTable[i].avcLevel) {
 *omxLevel = ConversionTable[i].omxLevel;
 return OK;
 }
 }

    ALOGE("ConvertAvcSpecLevelToOmxAvcLevel: %d level not supported",
 (int32_t) avcLevel);

 return BAD_VALUE;
}
