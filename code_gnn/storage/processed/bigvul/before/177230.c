static OMX_VIDEO_CONTROLRATETYPE getBitrateMode(const sp<AMessage> &msg) {
 int32_t tmp;
 if (!msg->findInt32("bitrate-mode", &tmp)) {
 return OMX_Video_ControlRateVariable;
 }

 return static_cast<OMX_VIDEO_CONTROLRATETYPE>(tmp);
}
