status_t OMXNodeInstance::setInternalOption(
        OMX_U32 portIndex,
        IOMX::InternalOptionType type,
 const void *data,
 size_t size) {
    CLOG_CONFIG(setInternalOption, "%s(%d): %s:%u %zu@%p",
            asString(type), type, portString(portIndex), portIndex, size, data);
 switch (type) {
 case IOMX::INTERNAL_OPTION_SUSPEND:
 case IOMX::INTERNAL_OPTION_REPEAT_PREVIOUS_FRAME_DELAY:
 case IOMX::INTERNAL_OPTION_MAX_TIMESTAMP_GAP:
 case IOMX::INTERNAL_OPTION_START_TIME:
 case IOMX::INTERNAL_OPTION_TIME_LAPSE:
 {
 const sp<GraphicBufferSource> &bufferSource =
                getGraphicBufferSource();

 if (bufferSource == NULL || portIndex != kPortIndexInput) {
                CLOGW("setInternalOption is only for Surface input");
 return ERROR_UNSUPPORTED;
 }

 if (type == IOMX::INTERNAL_OPTION_SUSPEND) {
 if (size != sizeof(bool)) {
 return INVALID_OPERATION;
 }

 bool suspend = *(bool *)data;
                CLOG_CONFIG(setInternalOption, "suspend=%d", suspend);
                bufferSource->suspend(suspend);
 } else if (type ==
                    IOMX::INTERNAL_OPTION_REPEAT_PREVIOUS_FRAME_DELAY){
 if (size != sizeof(int64_t)) {
 return INVALID_OPERATION;
 }

 int64_t delayUs = *(int64_t *)data;
                CLOG_CONFIG(setInternalOption, "delayUs=%lld", (long long)delayUs);
 return bufferSource->setRepeatPreviousFrameDelayUs(delayUs);
 } else if (type ==
                    IOMX::INTERNAL_OPTION_MAX_TIMESTAMP_GAP){
 if (size != sizeof(int64_t)) {
 return INVALID_OPERATION;
 }

 int64_t maxGapUs = *(int64_t *)data;
                CLOG_CONFIG(setInternalOption, "gapUs=%lld", (long long)maxGapUs);
 return bufferSource->setMaxTimestampGapUs(maxGapUs);
 } else if (type == IOMX::INTERNAL_OPTION_START_TIME) {
 if (size != sizeof(int64_t)) {
 return INVALID_OPERATION;
 }

 int64_t skipFramesBeforeUs = *(int64_t *)data;
                CLOG_CONFIG(setInternalOption, "beforeUs=%lld", (long long)skipFramesBeforeUs);
                bufferSource->setSkipFramesBeforeUs(skipFramesBeforeUs);
 } else {  
 if (size != sizeof(int64_t) * 2) {
 return INVALID_OPERATION;
 }

 int64_t timePerFrameUs = ((int64_t *)data)[0];
 int64_t timePerCaptureUs = ((int64_t *)data)[1];
                CLOG_CONFIG(setInternalOption, "perFrameUs=%lld perCaptureUs=%lld",
 (long long)timePerFrameUs, (long long)timePerCaptureUs);

                bufferSource->setTimeLapseUs((int64_t *)data);
 }

 return OK;
 }

 default:
 return ERROR_UNSUPPORTED;
 }
}
