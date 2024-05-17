OMX_ERRORTYPE omx_vdec::createDivxDrmContext()
{
    OMX_ERRORTYPE err = OMX_ErrorNone;
    iDivXDrmDecrypt = DivXDrmDecrypt::Create();
 if (iDivXDrmDecrypt) {
        OMX_ERRORTYPE err = iDivXDrmDecrypt->Init();
 if (err!=OMX_ErrorNone) {
            DEBUG_PRINT_ERROR("ERROR :iDivXDrmDecrypt->Init %d", err);
 delete iDivXDrmDecrypt;
            iDivXDrmDecrypt = NULL;
 }
 } else {
        DEBUG_PRINT_ERROR("Unable to Create DIVX DRM");
        err = OMX_ErrorUndefined;
 }
 return err;
}
