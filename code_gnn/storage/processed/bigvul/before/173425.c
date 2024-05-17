OMX_ERRORTYPE copy_buffer(OMX_BUFFERHEADERTYPE* pDst, OMX_BUFFERHEADERTYPE* pSrc)
{
    OMX_ERRORTYPE rc = OMX_ErrorNone;
 if ((pDst->nAllocLen - pDst->nFilledLen) >= pSrc->nFilledLen) {
        memcpy((pDst->pBuffer + pDst->nFilledLen), pSrc->pBuffer, pSrc->nFilledLen);
 if (pDst->nTimeStamp == LLONG_MAX) {
            pDst->nTimeStamp = pSrc->nTimeStamp;
            DEBUG_PRINT_LOW("Assign Dst nTimeStamp = %lld", pDst->nTimeStamp);
 }
        pDst->nFilledLen += pSrc->nFilledLen;
        pSrc->nFilledLen = 0;
 } else {
        DEBUG_PRINT_ERROR("Error: Destination buffer overflow");
        rc = OMX_ErrorBadParameter;
 }
 return rc;
}
