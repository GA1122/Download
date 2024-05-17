H264SwDecRet H264SwDecDecode(H264SwDecInst decInst, H264SwDecInput *pInput,
                  H264SwDecOutput *pOutput)
{

 decContainer_t *pDecCont;
    u32 strmLen;
    u32 numReadBytes;
    u8 *tmpStream;
    u32 decResult = 0;
    H264SwDecRet returnValue = H264SWDEC_STRM_PROCESSED;

    DEC_API_TRC("H264SwDecDecode#");

  
 if (pInput == NULL || pOutput == NULL)
 {
        DEC_API_TRC("H264SwDecDecode# ERROR: pInput or pOutput is NULL");
 return(H264SWDEC_PARAM_ERR);
 }

 if ((pInput->pStream == NULL) || (pInput->dataLen == 0))
 {
        DEC_API_TRC("H264SwDecDecode# ERROR: Invalid input parameters");
 return(H264SWDEC_PARAM_ERR);
 }

    pDecCont = (decContainer_t *)decInst;

  
 if (decInst == NULL || pDecCont->decStat == UNINITIALIZED)
 {
        DEC_API_TRC("H264SwDecDecode# ERROR: Decoder not initialized");
 return(H264SWDEC_NOT_INITIALIZED);
 }

#ifdef H264DEC_EVALUATION
 if (pDecCont->picNumber >= H264DEC_EVALUATION_LIMIT)
 return(H264SWDEC_EVALUATION_LIMIT_EXCEEDED);
#endif

#ifdef H264DEC_TRACE
    sprintf(pDecCont->str, "H264SwDecDecode# decInst %p  pInput %p  pOutput %p",
            decInst, (void*)pInput, (void*)pOutput);
    DEC_API_TRC(pDecCont->str);
#endif

    pOutput->pStrmCurrPos   = NULL;

    numReadBytes = 0;
    strmLen = pInput->dataLen;
    tmpStream = pInput->pStream;
    pDecCont->storage.intraConcealmentFlag = pInput->intraConcealmentMethod;

 do
 {
  
 if (pDecCont->decStat == NEW_HEADERS)
 {
            decResult = H264BSD_HDRS_RDY;
            pDecCont->decStat = INITIALIZED;
 }
 else  
 {
            decResult = h264bsdDecode(&pDecCont->storage, tmpStream, strmLen,
                pInput->picId, &numReadBytes);
 }
        tmpStream += numReadBytes;
  
 if ( (i32)(strmLen - numReadBytes) >= 0 )
            strmLen -= numReadBytes;
 else
            strmLen = 0;

        pOutput->pStrmCurrPos = tmpStream;

 switch (decResult)
 {
 case H264BSD_HDRS_RDY:

 if(pDecCont->storage.dpb->flushed &&
                   pDecCont->storage.dpb->numOut !=
                   pDecCont->storage.dpb->outIndex)
 {
  
                    pDecCont->storage.dpb->flushed = 0;
                    pDecCont->decStat = NEW_HEADERS;
                    returnValue = H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY;
                    strmLen = 0;
 }
 else
 {
                    returnValue = H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY;
                    strmLen = 0;
 }
 break;

 case H264BSD_PIC_RDY:
                pDecCont->picNumber++;

 if (strmLen == 0)
                    returnValue = H264SWDEC_PIC_RDY;
 else
                    returnValue = H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY;

                strmLen = 0;
 break;

 case H264BSD_PARAM_SET_ERROR:
 if ( !h264bsdCheckValidParamSets(&pDecCont->storage) &&
                     strmLen == 0 )
 {
                    returnValue = H264SWDEC_STRM_ERR;
 }
 break;
 case H264BSD_MEMALLOC_ERROR:
 {
                    returnValue = H264SWDEC_MEMFAIL;
                    strmLen = 0;
 }
 break;
 default:
 break;
 }

 } while (strmLen);

#ifdef H264DEC_TRACE
    sprintf(pDecCont->str, "H264SwDecDecode# OK: DecResult %d",
            returnValue);
    DEC_API_TRC(pDecCont->str);
#endif

 return(returnValue);

}
