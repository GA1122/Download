H264SwDecRet H264SwDecGetInfo(H264SwDecInst decInst, H264SwDecInfo *pDecInfo)
{

 storage_t *pStorage;

    DEC_API_TRC("H264SwDecGetInfo#");

 if (decInst == NULL || pDecInfo == NULL)
 {
        DEC_API_TRC("H264SwDecGetInfo# ERROR: decInst or pDecInfo is NULL");
 return(H264SWDEC_PARAM_ERR);
 }

    pStorage = &(((decContainer_t *)decInst)->storage);

 if (pStorage->activeSps == NULL || pStorage->activePps == NULL)
 {
        DEC_API_TRC("H264SwDecGetInfo# ERROR: Headers not decoded yet");
 return(H264SWDEC_HDRS_NOT_RDY);
 }

#ifdef H264DEC_TRACE
    sprintf(((decContainer_t*)decInst)->str,
 "H264SwDecGetInfo# decInst %p  pDecInfo %p", decInst, (void*)pDecInfo);
    DEC_API_TRC(((decContainer_t*)decInst)->str);
#endif

  
    pDecInfo->picWidth        = h264bsdPicWidth(pStorage) << 4;
    pDecInfo->picHeight       = h264bsdPicHeight(pStorage) << 4;
    pDecInfo->videoRange      = h264bsdVideoRange(pStorage);
    pDecInfo->matrixCoefficients = h264bsdMatrixCoefficients(pStorage);

    h264bsdCroppingParams(pStorage,
 &pDecInfo->croppingFlag,
 &pDecInfo->cropParams.cropLeftOffset,
 &pDecInfo->cropParams.cropOutWidth,
 &pDecInfo->cropParams.cropTopOffset,
 &pDecInfo->cropParams.cropOutHeight);

  
    h264bsdSampleAspectRatio(pStorage,
 &pDecInfo->parWidth,
 &pDecInfo->parHeight);

  
    pDecInfo->profile = h264bsdProfile(pStorage);

    DEC_API_TRC("H264SwDecGetInfo# OK");

 return(H264SWDEC_OK);

}
