OMX_ERRORTYPE omx_video::free_output_buffer(OMX_BUFFERHEADERTYPE *bufferHdr)
{
 unsigned int index = 0;
    OMX_U8 *temp_buff ;

 if (bufferHdr == NULL || m_out_mem_ptr == NULL) {
        DEBUG_PRINT_ERROR("ERROR: free_output: Invalid bufferHdr[%p] or m_out_mem_ptr[%p]",
                bufferHdr, m_out_mem_ptr);
 return OMX_ErrorBadParameter;
 }
    index = bufferHdr - m_out_mem_ptr;

 if (index < m_sOutPortDef.nBufferCountActual &&
            dev_free_buf(&m_pOutput_pmem[index],PORT_INDEX_OUT) != true) {
        DEBUG_PRINT_ERROR("ERROR: dev_free_buf Failed for o/p buf");
 }

 if (index < m_sOutPortDef.nBufferCountActual && m_pOutput_pmem) {
 if (m_pOutput_pmem[index].fd > 0 && output_use_buffer == false ) {
            DEBUG_PRINT_LOW("FreeBuffer:: o/p AllocateBuffer case");
 if(!secure_session) {
                munmap (m_pOutput_pmem[index].buffer,
                        m_pOutput_pmem[index].size);
 } else {
 char *data = (char*) m_pOutput_pmem[index].buffer;
 native_handle_t *handle = NULL;
                memcpy(&handle, data + sizeof(OMX_U32), sizeof(native_handle_t*));
                native_handle_delete(handle);
                free(m_pOutput_pmem[index].buffer);
 }
            close (m_pOutput_pmem[index].fd);
#ifdef USE_ION
            free_ion_memory(&m_pOutput_ion[index]);
#endif
            m_pOutput_pmem[index].fd = -1;
 } else if ( m_pOutput_pmem[index].fd > 0 && (output_use_buffer == true
 && m_use_output_pmem == OMX_FALSE)) {
            DEBUG_PRINT_LOW("FreeBuffer:: o/p Heap UseBuffer case");
 if (dev_free_buf(&m_pOutput_pmem[index],PORT_INDEX_OUT) != true) {
                DEBUG_PRINT_ERROR("ERROR: dev_free_buf Failed for o/p buf");
 }
 if(!secure_session) {
                munmap (m_pOutput_pmem[index].buffer,
                        m_pOutput_pmem[index].size);
 }
            close (m_pOutput_pmem[index].fd);
#ifdef USE_ION
            free_ion_memory(&m_pOutput_ion[index]);
#endif
            m_pOutput_pmem[index].fd = -1;
 } else {
            DEBUG_PRINT_LOW("FreeBuffer:: fd is invalid or o/p PMEM UseBuffer case");
 }
 }
 return OMX_ErrorNone;
}
