status_t ACodec::verifySupportForProfileAndLevel(
 int32_t profile, int32_t level) {
    OMX_VIDEO_PARAM_PROFILELEVELTYPE params;
 InitOMXParams(&params);
    params.nPortIndex = kPortIndexOutput;

 for (params.nProfileIndex = 0;; ++params.nProfileIndex) {
 status_t err = mOMX->getParameter(
                mNode,
                OMX_IndexParamVideoProfileLevelQuerySupported,
 &params,
 sizeof(params));

 if (err != OK) {
 return err;
 }

 int32_t supportedProfile = static_cast<int32_t>(params.eProfile);
 int32_t supportedLevel = static_cast<int32_t>(params.eLevel);

 if (profile == supportedProfile && level <= supportedLevel) {
 return OK;
 }
 }
}
