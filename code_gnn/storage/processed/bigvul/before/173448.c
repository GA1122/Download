void omx_vdec::free_output_buffer_header()
{
    DEBUG_PRINT_HIGH("ALL output buffers are freed/released");
    output_use_buffer = false;
    ouput_egl_buffers = false;

 if (m_out_mem_ptr) {
        free (m_out_mem_ptr);
        m_out_mem_ptr = NULL;
 }

 if (m_platform_list) {
        free(m_platform_list);
        m_platform_list = NULL;
 }

 if (drv_ctx.ptr_respbuffer) {
        free (drv_ctx.ptr_respbuffer);
        drv_ctx.ptr_respbuffer = NULL;
 }
 if (drv_ctx.ptr_outputbuffer) {
        free (drv_ctx.ptr_outputbuffer);
        drv_ctx.ptr_outputbuffer = NULL;
 }
#ifdef USE_ION
 if (drv_ctx.op_buf_ion_info) {
        DEBUG_PRINT_LOW("Free o/p ion context");
        free(drv_ctx.op_buf_ion_info);
        drv_ctx.op_buf_ion_info = NULL;
 }
#endif
 if (out_dynamic_list) {
        free(out_dynamic_list);
        out_dynamic_list = NULL;
 }
}
