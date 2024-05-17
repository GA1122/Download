status_t QueryCodec(
 const sp<IOMX> &omx,
 const char *componentName, const char *mime,
 bool isEncoder,
 CodecCapabilities *caps) {
 bool isVideo = !strncasecmp(mime, "video/", 6);

    sp<OMXCodecObserver> observer = new OMXCodecObserver;
    IOMX::node_id node;
 status_t err = omx->allocateNode(componentName, observer, &node);

 if (err != OK) {
 return err;
 }

 OMXCodec::setComponentRole(omx, node, isEncoder, mime);

    caps->mFlags = 0;
    caps->mComponentName = componentName;

 if (isVideo) {
        OMX_VIDEO_PARAM_PROFILELEVELTYPE param;
 InitOMXParams(&param);

        param.nPortIndex = !isEncoder ? 0 : 1;

 for (param.nProfileIndex = 0;; ++param.nProfileIndex) {
            err = omx->getParameter(
                    node, OMX_IndexParamVideoProfileLevelQuerySupported,
 &param, sizeof(param));

 if (err != OK) {
 break;
 }

 CodecProfileLevel profileLevel;
            profileLevel.mProfile = param.eProfile;
            profileLevel.mLevel = param.eLevel;

            caps->mProfileLevels.push(profileLevel);
 }

        OMX_VIDEO_PARAM_PORTFORMATTYPE portFormat;
 InitOMXParams(&portFormat);
        portFormat.nPortIndex = !isEncoder ? 1 : 0;
 for (portFormat.nIndex = 0;; ++portFormat.nIndex) {
            err = omx->getParameter(
                    node, OMX_IndexParamVideoPortFormat,
 &portFormat, sizeof(portFormat));
 if (err != OK) {
 break;
 }

            OMX_U32 flexibleEquivalent;
 if (ACodec::isFlexibleColorFormat(
                        omx, node, portFormat.eColorFormat, false  ,
 &flexibleEquivalent)) {
 bool marked = false;
 for (size_t i = 0; i < caps->mColorFormats.size(); i++) {
 if (caps->mColorFormats.itemAt(i) == flexibleEquivalent) {
                        marked = true;
 break;
 }
 }
 if (!marked) {
                    caps->mColorFormats.push(flexibleEquivalent);
 }
 }
            caps->mColorFormats.push(portFormat.eColorFormat);
 }
 }

 if (isVideo && !isEncoder) {
 if (omx->storeMetaDataInBuffers(
                    node, 1  , OMX_TRUE) == OK ||
            omx->prepareForAdaptivePlayback(
                    node, 1  , OMX_TRUE,
 1280  , 720  ) == OK) {
            caps->mFlags |= CodecCapabilities::kFlagSupportsAdaptivePlayback;
 }
 }

    CHECK_EQ(omx->freeNode(node), (status_t)OK);

 return OK;
}
