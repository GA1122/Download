OMX_ERRORTYPE omx_vdec::free_output_buffer(OMX_BUFFERHEADERTYPE *bufferHdr)
{
 unsigned int index = 0;

 if (bufferHdr == NULL || m_out_mem_ptr == NULL) {
 return OMX_ErrorBadParameter;
 }

    index = bufferHdr - m_out_mem_ptr;
    DEBUG_PRINT_LOW("Free ouput Buffer index = %d",index);

 if (index < drv_ctx.op_buf.actualcount
 && drv_ctx.ptr_outputbuffer) {
        DEBUG_PRINT_LOW("Free ouput Buffer index = %d addr = %p", index,
                drv_ctx.ptr_outputbuffer[index].bufferaddr);

 struct vdec_setbuffer_cmd setbuffers;
        setbuffers.buffer_type = VDEC_BUFFER_TYPE_OUTPUT;
        memcpy (&setbuffers.buffer,&drv_ctx.ptr_outputbuffer[index],
 sizeof (vdec_bufferpayload));

 if (!dynamic_buf_mode) {
#ifdef _ANDROID_
 if (m_enable_android_native_buffers) {
 if (!secure_mode) {
 if (drv_ctx.ptr_outputbuffer[index].pmem_fd > 0) {
                        munmap(drv_ctx.ptr_outputbuffer[index].bufferaddr,
                                drv_ctx.ptr_outputbuffer[index].mmaped_size);
 }
 }
                drv_ctx.ptr_outputbuffer[index].pmem_fd = -1;
 } else {
#endif
 if (drv_ctx.ptr_outputbuffer[0].pmem_fd > 0 && !ouput_egl_buffers && !m_use_output_pmem) {
 if (!secure_mode) {
                        DEBUG_PRINT_LOW("unmap the output buffer fd = %d",
                                drv_ctx.ptr_outputbuffer[0].pmem_fd);
                        DEBUG_PRINT_LOW("unmap the ouput buffer size=%u  address = %p",
 (unsigned int)drv_ctx.ptr_outputbuffer[0].mmaped_size * drv_ctx.op_buf.actualcount,
                                drv_ctx.ptr_outputbuffer[0].bufferaddr);
                        munmap (drv_ctx.ptr_outputbuffer[0].bufferaddr,
                                drv_ctx.ptr_outputbuffer[0].mmaped_size * drv_ctx.op_buf.actualcount);
 }
                    close (drv_ctx.ptr_outputbuffer[0].pmem_fd);
                    drv_ctx.ptr_outputbuffer[0].pmem_fd = -1;
#ifdef USE_ION
                    free_ion_memory(&drv_ctx.op_buf_ion_info[0]);
#endif
 }
#ifdef _ANDROID_
 }
#endif
 }  
 if (release_output_done()) {
            free_extradata();
 }
 }

 return OMX_ErrorNone;

}
