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
 case IOMX::INTERNAL_OPTION_MAX_FPS:
 case IOMX::INTERNAL_OPTION_START_TIME:
 case IOMX::INTERNAL_OPTION_TIME_LAPSE:
 case IOMX::INTERNAL_OPTION_COLOR_ASPECTS:
 {
 const sp<GraphicBufferSource> &bufferSource =
                getGraphicBufferSource();

 if (bufferSource == NULL || portIndex != kPortIndexInput) {
                CLOGW("setInternalOption is only for Surface input");
 return ERROR_UNSUPPORTED;
 }

 if (type == IOMX::INTERNAL_OPTION_SUSPEND) {
 bool suspend;
 if (!getInternalOption(data, size, &suspend)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "suspend=%d", suspend);
                bufferSource->suspend(suspend);
 } else if (type == IOMX::INTERNAL_OPTION_REPEAT_PREVIOUS_FRAME_DELAY) {
 int64_t delayUs;
 if (!getInternalOption(data, size, &delayUs)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "delayUs=%lld", (long long)delayUs);
 return bufferSource->setRepeatPreviousFrameDelayUs(delayUs);
 } else if (type == IOMX::INTERNAL_OPTION_MAX_TIMESTAMP_GAP) {
 int64_t maxGapUs;
 if (!getInternalOption(data, size, &maxGapUs)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "gapUs=%lld", (long long)maxGapUs);
 return bufferSource->setMaxTimestampGapUs(maxGapUs);
 } else if (type == IOMX::INTERNAL_OPTION_MAX_FPS) {
 float maxFps;
 if (!getInternalOption(data, size, &maxFps)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "maxFps=%f", maxFps);
 return bufferSource->setMaxFps(maxFps);
 } else if (type == IOMX::INTERNAL_OPTION_START_TIME) {
 int64_t skipFramesBeforeUs;
 if (!getInternalOption(data, size, &skipFramesBeforeUs)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "beforeUs=%lld", (long long)skipFramesBeforeUs);
                bufferSource->setSkipFramesBeforeUs(skipFramesBeforeUs);
 } else if (type == IOMX::INTERNAL_OPTION_TIME_LAPSE) {
 GraphicBufferSource::TimeLapseConfig config;
 if (!getInternalOption(data, size, &config)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "perFrameUs=%lld perCaptureUs=%lld",
 (long long)config.mTimePerFrameUs, (long long)config.mTimePerCaptureUs);

 return bufferSource->setTimeLapseConfig(config);
 } else if (type == IOMX::INTERNAL_OPTION_COLOR_ASPECTS) {
 ColorAspects aspects;
 if (!getInternalOption(data, size, &aspects)) {
 return INVALID_OPERATION;
 }

                CLOG_CONFIG(setInternalOption, "setting color aspects");
                bufferSource->setColorAspects(aspects);
 }

 return OK;
 }

 default:
 return ERROR_UNSUPPORTED;
 }
}
