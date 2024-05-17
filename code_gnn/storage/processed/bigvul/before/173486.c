OMX_ERRORTYPE omx_vdec::push_input_vc1 (OMX_HANDLETYPE hComp)
{
    OMX_U8 *buf, *pdest;
    OMX_U32 partial_frame = 1;
    OMX_U32 buf_len, dest_len;

 if (first_frame == 0) {
        first_frame = 1;
        DEBUG_PRINT_LOW("First i/p buffer for VC1 arbitrary bytes");
 if (!m_vendor_config.pData) {
            DEBUG_PRINT_LOW("Check profile type in 1st source buffer");
            buf = psource_frame->pBuffer;
            buf_len = psource_frame->nFilledLen;

 if ((*((OMX_U32 *) buf) & VC1_SP_MP_START_CODE_MASK) ==
                    VC1_SP_MP_START_CODE) {
                m_vc1_profile = VC1_SP_MP_RCV;
 } else if (*((OMX_U32 *) buf) & VC1_AP_SEQ_START_CODE) {
                m_vc1_profile = VC1_AP;
 } else {
                DEBUG_PRINT_ERROR("Invalid sequence layer in first buffer");
 return OMX_ErrorStreamCorrupt;
 }
 } else {
            pdest = pdest_frame->pBuffer + pdest_frame->nFilledLen +
                pdest_frame->nOffset;
            dest_len = pdest_frame->nAllocLen - (pdest_frame->nFilledLen +
                    pdest_frame->nOffset);

 if (dest_len < m_vendor_config.nDataSize) {
                DEBUG_PRINT_ERROR("Destination buffer full");
 return OMX_ErrorBadParameter;
 } else {
                memcpy(pdest, m_vendor_config.pData, m_vendor_config.nDataSize);
                pdest_frame->nFilledLen += m_vendor_config.nDataSize;
 }
 }
 }

 switch (m_vc1_profile) {
 case VC1_AP:
            DEBUG_PRINT_LOW("VC1 AP, hence parse using frame start code");
 if (push_input_sc_codec(hComp) != OMX_ErrorNone) {
                DEBUG_PRINT_ERROR("Error In Parsing VC1 AP start code");
 return OMX_ErrorBadParameter;
 }
 break;

 case VC1_SP_MP_RCV:
 default:
            DEBUG_PRINT_ERROR("Unsupported VC1 profile in ArbitraryBytes Mode");
 return OMX_ErrorBadParameter;
 }
 return OMX_ErrorNone;
}
