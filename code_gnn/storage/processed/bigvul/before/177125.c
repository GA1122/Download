static status_t ConvertOmxAvcLevelToAvcSpecLevel(
        OMX_VIDEO_AVCLEVELTYPE omxLevel, WORD32 *avcLevel) {
 for (size_t i = 0; i < NELEM(ConversionTable); ++i) {
 if (omxLevel == ConversionTable[i].omxLevel) {
 *avcLevel = ConversionTable[i].avcLevel;
 return OK;
 }
 }

    ALOGE("ConvertOmxAvcLevelToAvcSpecLevel: %d level not supported",
 (int32_t)omxLevel);

 return BAD_VALUE;
}
