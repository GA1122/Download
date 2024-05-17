status_t ACodec::setParameters(const sp<AMessage> &params) {
 int32_t videoBitrate;
 if (params->findInt32("video-bitrate", &videoBitrate)) {
        OMX_VIDEO_CONFIG_BITRATETYPE configParams;
 InitOMXParams(&configParams);
        configParams.nPortIndex = kPortIndexOutput;
        configParams.nEncodeBitrate = videoBitrate;

 status_t err = mOMX->setConfig(
                mNode,
                OMX_IndexConfigVideoBitrate,
 &configParams,
 sizeof(configParams));

 if (err != OK) {
            ALOGE("setConfig(OMX_IndexConfigVideoBitrate, %d) failed w/ err %d",
                   videoBitrate, err);

 return err;
 }
 }

 int64_t skipFramesBeforeUs;
 if (params->findInt64("skip-frames-before", &skipFramesBeforeUs)) {
 status_t err =
            mOMX->setInternalOption(
                     mNode,
                     kPortIndexInput,
                     IOMX::INTERNAL_OPTION_START_TIME,
 &skipFramesBeforeUs,
 sizeof(skipFramesBeforeUs));

 if (err != OK) {
            ALOGE("Failed to set parameter 'skip-frames-before' (err %d)", err);
 return err;
 }
 }

 int32_t dropInputFrames;
 if (params->findInt32("drop-input-frames", &dropInputFrames)) {
 bool suspend = dropInputFrames != 0;

 status_t err =
            mOMX->setInternalOption(
                     mNode,
                     kPortIndexInput,
                     IOMX::INTERNAL_OPTION_SUSPEND,
 &suspend,
 sizeof(suspend));

 if (err != OK) {
            ALOGE("Failed to set parameter 'drop-input-frames' (err %d)", err);
 return err;
 }
 }

 int32_t dummy;
 if (params->findInt32("request-sync", &dummy)) {
 status_t err = requestIDRFrame();

 if (err != OK) {
            ALOGE("Requesting a sync frame failed w/ err %d", err);
 return err;
 }
 }

 float rate;
 if (params->findFloat("operating-rate", &rate) && rate > 0) {
 status_t err = setOperatingRate(rate, mIsVideo);
 if (err != OK) {
            ALOGE("Failed to set parameter 'operating-rate' (err %d)", err);
 return err;
 }
 }

 return OK;
}
