bool omx_vdec::allocate_input_done(void)
{
 bool bRet = false;
 unsigned i=0;

 if (m_inp_mem_ptr == NULL) {
 return bRet;
 }
 if (m_inp_mem_ptr ) {
 for (; i<drv_ctx.ip_buf.actualcount; i++) {
 if (BITMASK_ABSENT(&m_inp_bm_count,i)) {
 break;
 }
 }
 }
 if (i == drv_ctx.ip_buf.actualcount) {
        bRet = true;
        DEBUG_PRINT_HIGH("Allocate done for all i/p buffers");
 }
 if (i==drv_ctx.ip_buf.actualcount && m_inp_bEnabled) {
        m_inp_bPopulated = OMX_TRUE;
 }
 return bRet;
}
