bool omx_video::release_input_done(void)
{
 bool bRet = false;
 unsigned i=0,j=0;

    DEBUG_PRINT_LOW("Inside release_input_done()");
 if (m_inp_mem_ptr) {
 for (; j<m_sInPortDef.nBufferCountActual; j++) {
 if ( BITMASK_PRESENT(&m_inp_bm_count,j)) {
 break;
 }
 }
 if (j==m_sInPortDef.nBufferCountActual) {
            bRet = true;
 }
 } else {
        bRet = true;
 }
 return bRet;
}
