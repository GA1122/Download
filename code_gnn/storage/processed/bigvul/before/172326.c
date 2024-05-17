bool omx_video::allocate_output_done(void)
{
 bool bRet = false;
 unsigned j=0;

 if (m_out_mem_ptr == NULL) {
 return bRet;
 }

 if (m_out_mem_ptr ) {
 for (; j<m_sOutPortDef.nBufferCountActual; j++) {
 if (BITMASK_ABSENT(&m_out_bm_count,j)) {
 break;
 }
 }
 }

 if (j==m_sOutPortDef.nBufferCountActual) {
        bRet = true;
 }

 if (j==m_sOutPortDef.nBufferCountActual && m_sOutPortDef.bEnabled) {
        m_sOutPortDef.bPopulated = OMX_TRUE;
 }
 return bRet;
}
