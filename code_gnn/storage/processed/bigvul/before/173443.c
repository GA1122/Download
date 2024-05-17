OMX_ERRORTYPE omx_vdec::free_input_buffer(unsigned int bufferindex,
        OMX_BUFFERHEADERTYPE *pmem_bufferHdr)
{
 if (m_inp_heap_ptr && !input_use_buffer && arbitrary_bytes) {
 if (m_inp_heap_ptr[bufferindex].pBuffer)
            free(m_inp_heap_ptr[bufferindex].pBuffer);
        m_inp_heap_ptr[bufferindex].pBuffer = NULL;
 }
 if (pmem_bufferHdr)
        free_input_buffer(pmem_bufferHdr);
 return OMX_ErrorNone;
}
