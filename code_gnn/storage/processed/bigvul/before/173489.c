bool omx_vdec::release_input_done(void)
{
 bool bRet = false;
 unsigned i=0,j=0;

    DEBUG_PRINT_LOW("Value of m_inp_mem_ptr %p",m_inp_mem_ptr);
 if (m_inp_mem_ptr) {
 for (; j<drv_ctx.ip_buf.actualcount; j++) {
 if ( BITMASK_PRESENT(&m_inp_bm_count,j)) {
 break;
 }
 }
 if (j==drv_ctx.ip_buf.actualcount) {
            bRet = true;
 }
 } else {
        bRet = true;
 }
 return bRet;
}
