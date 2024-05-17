bool omx_vdec::allocate_output_done(void)
{
 bool bRet = false;
 unsigned j=0;

 if (m_out_mem_ptr == NULL) {
 return bRet;
 }

 if (m_out_mem_ptr) {
 for (; j < drv_ctx.op_buf.actualcount; j++) {
 if (BITMASK_ABSENT(&m_out_bm_count,j)) {
 break;
 }
 }
 }

 if (j == drv_ctx.op_buf.actualcount) {
        bRet = true;
        DEBUG_PRINT_HIGH("Allocate done for all o/p buffers");
 if (m_out_bEnabled)
            m_out_bPopulated = OMX_TRUE;
 }

 return bRet;
}
