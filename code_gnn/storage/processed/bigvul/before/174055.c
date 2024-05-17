H264SwDecRet H264SwDecNextPicture(H264SwDecInst decInst,
    H264SwDecPicture *pOutput, u32 flushBuffer)
{

 decContainer_t *pDecCont;
    u32 numErrMbs, isIdrPic, picId;
    u32 *pOutPic;

    DEC_API_TRC("H264SwDecNextPicture#");

 if (decInst == NULL || pOutput == NULL)
 {
        DEC_API_TRC("H264SwDecNextPicture# ERROR: decInst or pOutput is NULL");
 return(H264SWDEC_PARAM_ERR);
 }

    pDecCont = (decContainer_t*)decInst;

#ifdef H264DEC_TRACE
    sprintf(pDecCont->str, "H264SwDecNextPicture# decInst %p pOutput %p %s %d",
            decInst, (void*)pOutput, "flushBuffer", flushBuffer);
    DEC_API_TRC(pDecCont->str);
#endif

 if (flushBuffer)
        h264bsdFlushBuffer(&pDecCont->storage);

    pOutPic = (u32*)h264bsdNextOutputPicture(&pDecCont->storage, &picId,
 &isIdrPic, &numErrMbs);

 if (pOutPic == NULL)
 {
        DEC_API_TRC("H264SwDecNextPicture# OK: return H264SWDEC_OK");
 return(H264SWDEC_OK);
 }
 else
 {
        pOutput->pOutputPicture = pOutPic;
        pOutput->picId          = picId;
        pOutput->isIdrPicture   = isIdrPic;
        pOutput->nbrOfErrMBs    = numErrMbs;
        DEC_API_TRC("H264SwDecNextPicture# OK: return H264SWDEC_PIC_RDY");
 return(H264SWDEC_PIC_RDY);
 }

}
