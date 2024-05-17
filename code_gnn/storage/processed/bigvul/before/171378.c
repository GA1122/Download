status_t OMXCodec::setupErrorCorrectionParameters() {
    OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE errorCorrectionType;
 InitOMXParams(&errorCorrectionType);
    errorCorrectionType.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoErrorCorrection,
 &errorCorrectionType, sizeof(errorCorrectionType));
 if (err != OK) {
        ALOGW("Error correction param query is not supported");
 return OK;  
 }

    errorCorrectionType.bEnableHEC = OMX_FALSE;
    errorCorrectionType.bEnableResync = OMX_TRUE;
    errorCorrectionType.nResynchMarkerSpacing = 256;
    errorCorrectionType.bEnableDataPartitioning = OMX_FALSE;
    errorCorrectionType.bEnableRVLC = OMX_FALSE;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoErrorCorrection,
 &errorCorrectionType, sizeof(errorCorrectionType));
 if (err != OK) {
        ALOGW("Error correction param configuration is not supported");
 }

 return OK;
}
