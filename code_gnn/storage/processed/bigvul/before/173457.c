    OMX_BUFFERHEADERTYPE* omx_vdec::allocate_color_convert_buf::get_il_buf_hdr
(OMX_BUFFERHEADERTYPE *bufadd)
{
 if (!omx) {
        DEBUG_PRINT_ERROR("Invalid param get_buf_hdr");
 return NULL;
 }
 if (!enabled)
 return bufadd;

 unsigned index = 0;
    index = bufadd - omx->m_out_mem_ptr;
 if (index < omx->drv_ctx.op_buf.actualcount) {
        m_out_mem_ptr_client[index].nFlags = (bufadd->nFlags & OMX_BUFFERFLAG_EOS);
        m_out_mem_ptr_client[index].nTimeStamp = bufadd->nTimeStamp;
 bool status;
 if (!omx->in_reconfig && !omx->output_flush_progress && bufadd->nFilledLen) {
            pthread_mutex_lock(&omx->c_lock);
            cache_clean_buffer(index);
            status = c2d.convert(omx->drv_ctx.ptr_outputbuffer[index].pmem_fd,
                    omx->m_out_mem_ptr->pBuffer, bufadd->pBuffer, pmem_fd[index],
                    pmem_baseaddress[index], pmem_baseaddress[index]);
 if (!status) {
                DEBUG_PRINT_ERROR("Failed color conversion %d", status);
                m_out_mem_ptr_client[index].nFilledLen = 0;
                pthread_mutex_unlock(&omx->c_lock);
 return &m_out_mem_ptr_client[index];
 } else {
 unsigned int filledLen = 0;
                c2d.get_output_filled_length(filledLen);
                m_out_mem_ptr_client[index].nFilledLen = filledLen;
                cache_clean_invalidate_buffer(index);
 }
            pthread_mutex_unlock(&omx->c_lock);
 } else
            m_out_mem_ptr_client[index].nFilledLen = 0;
 return &m_out_mem_ptr_client[index];
 }
    DEBUG_PRINT_ERROR("Index messed up in the get_il_buf_hdr");
 return NULL;
}
