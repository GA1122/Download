void omx_vdec::free_input_buffer_header()
{
    input_use_buffer = false;
 if (arbitrary_bytes) {
 if (m_inp_heap_ptr) {
            DEBUG_PRINT_LOW("Free input Heap Pointer");
            free (m_inp_heap_ptr);
            m_inp_heap_ptr = NULL;
 }

 if (m_phdr_pmem_ptr) {
            DEBUG_PRINT_LOW("Free input pmem header Pointer");
            free (m_phdr_pmem_ptr);
            m_phdr_pmem_ptr = NULL;
 }
 }
 if (m_inp_mem_ptr) {
        DEBUG_PRINT_LOW("Free input pmem Pointer area");
        free (m_inp_mem_ptr);
        m_inp_mem_ptr = NULL;
 }
  
 while (m_input_free_q.m_size) {
 unsigned long address, p2, id;
        m_input_free_q.pop_entry(&address, &p2, &id);
 }
 while (m_input_pending_q.m_size) {
 unsigned long address, p2, id;
        m_input_pending_q.pop_entry(&address, &p2, &id);
 }
    pdest_frame = NULL;
    psource_frame = NULL;
 if (drv_ctx.ptr_inputbuffer) {
        DEBUG_PRINT_LOW("Free Driver Context pointer");
        free (drv_ctx.ptr_inputbuffer);
        drv_ctx.ptr_inputbuffer = NULL;
 }
#ifdef USE_ION
 if (drv_ctx.ip_buf_ion_info) {
        DEBUG_PRINT_LOW("Free ion context");
        free(drv_ctx.ip_buf_ion_info);
        drv_ctx.ip_buf_ion_info = NULL;
 }
#endif
}
