bool omx_video::allocate_input_done(void)
{
 bool bRet = false;
 unsigned i=0;

 if (m_inp_mem_ptr == NULL) {
 return bRet;
 }
 if (m_inp_mem_ptr ) {
 for (; i<m_sInPortDef.nBufferCountActual; i++) {
 if (BITMASK_ABSENT(&m_inp_bm_count,i)) {
 break;
 }
 }
 }
 if (i==m_sInPortDef.nBufferCountActual) {
        bRet = true;
 }
 if (i==m_sInPortDef.nBufferCountActual && m_sInPortDef.bEnabled) {
        m_sInPortDef.bPopulated = OMX_TRUE;
 }
 return bRet;
}
