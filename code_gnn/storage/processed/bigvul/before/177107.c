static status_t ConvertOmxAvcLevelToAvcSpecLevel(
        OMX_U32 omxLevel, AVCLevel *avcLevel) {
 for (size_t i = 0, n = sizeof(ConversionTable)/sizeof(ConversionTable[0]);
        i < n; ++i) {
 if (omxLevel == ConversionTable[i].omxLevel) {
 *avcLevel = ConversionTable[i].avcLevel;
 return OK;
 }
 }

    ALOGE("ConvertOmxAvcLevelToAvcSpecLevel: %d level not supported",
 (int32_t)omxLevel);

 return BAD_VALUE;
}
