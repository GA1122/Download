    OMX_BUFFERHEADERTYPE* omx_vdec::allocate_color_convert_buf::get_dr_buf_hdr
(OMX_BUFFERHEADERTYPE *bufadd)
{
 if (!omx) {
        DEBUG_PRINT_ERROR("Invalid param get_buf_hdr");
 return NULL;
 }
 if (!enabled)
 return bufadd;
 unsigned index = 0;
    index = bufadd - m_out_mem_ptr_client;
 if (index < omx->drv_ctx.op_buf.actualcount) {
 return &omx->m_out_mem_ptr[index];
 }
    DEBUG_PRINT_ERROR("Index messed up in the get_dr_buf_hdr");
 return NULL;
}
