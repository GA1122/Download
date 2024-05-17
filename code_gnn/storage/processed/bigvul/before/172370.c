OMX_ERRORTYPE omx_video::push_input_buffer(OMX_HANDLETYPE hComp)
{
 unsigned long address = 0,p2,id, index = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    DEBUG_PRINT_LOW("In push input buffer");
 if (!psource_frame && m_opq_meta_q.m_size) {
        m_opq_meta_q.pop_entry(&address,&p2,&id);
        psource_frame = (OMX_BUFFERHEADERTYPE* ) address;
 }
 if (!pdest_frame && m_opq_pmem_q.m_size) {
        m_opq_pmem_q.pop_entry(&address,&p2,&id);
        pdest_frame = (OMX_BUFFERHEADERTYPE* ) address;
 }
 while (psource_frame != NULL && pdest_frame != NULL &&
            ret == OMX_ErrorNone) {
 struct pmem Input_pmem_info;
        encoder_media_buffer_type *media_buffer;
        index = pdest_frame - m_inp_mem_ptr;
 if (index >= m_sInPortDef.nBufferCountActual) {
            DEBUG_PRINT_ERROR("Output buffer index is wrong %u act count %u",
 (unsigned int)index, (unsigned int)m_sInPortDef.nBufferCountActual);
 return OMX_ErrorBadParameter;
 }

 if (psource_frame->nFilledLen == 0 && (psource_frame->nFlags & OMX_BUFFERFLAG_EOS)) {
 return push_empty_eos_buffer(hComp, psource_frame);
 }
        media_buffer = (encoder_media_buffer_type *)psource_frame->pBuffer;
  
 if (media_buffer->buffer_type == kMetadataBufferTypeCameraSource) {
 Input_pmem_info.buffer = media_buffer;
 Input_pmem_info.fd = media_buffer->meta_handle->data[0];
 Input_pmem_info.offset = media_buffer->meta_handle->data[1];
 Input_pmem_info.size = media_buffer->meta_handle->data[2];
            DEBUG_PRINT_LOW("ETB fd = %d, offset = %d, size = %d",Input_pmem_info.fd,
 Input_pmem_info.offset,
 Input_pmem_info.size);
            ret = queue_meta_buffer(hComp,Input_pmem_info);
 } else {
 private_handle_t *handle = (private_handle_t *)media_buffer->meta_handle;
 Input_pmem_info.buffer = media_buffer;
 Input_pmem_info.fd = handle->fd;
 Input_pmem_info.offset = 0;
 Input_pmem_info.size = handle->size;
 if (handle->format == HAL_PIXEL_FORMAT_RGBA_8888)
                ret = convert_queue_buffer(hComp,Input_pmem_info,index);
 else if (handle->format == HAL_PIXEL_FORMAT_NV12_ENCODEABLE ||
                    handle->format == QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m)
                ret = queue_meta_buffer(hComp,Input_pmem_info);
 else
                ret = OMX_ErrorBadParameter;
 }
 }
 return ret;
}
