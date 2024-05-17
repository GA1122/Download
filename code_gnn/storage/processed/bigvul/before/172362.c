void omx_video::omx_release_meta_buffer(OMX_BUFFERHEADERTYPE *buffer)
{
 if (buffer && meta_mode_enable) {
        encoder_media_buffer_type *media_ptr;
 struct pmem Input_pmem;
 unsigned int index_pmem = 0;
 bool meta_error = false;

        index_pmem = (buffer - m_inp_mem_ptr);
 if (mUsesColorConversion &&
 (index_pmem < m_sInPortDef.nBufferCountActual)) {
 if (!dev_free_buf((&m_pInput_pmem[index_pmem]),PORT_INDEX_IN)) {
                DEBUG_PRINT_ERROR("omx_release_meta_buffer dev free failed");
 }
 } else {
            media_ptr = (encoder_media_buffer_type *) buffer->pBuffer;
 if (media_ptr && media_ptr->meta_handle) {
 if (media_ptr->buffer_type == kMetadataBufferTypeCameraSource &&
                        media_ptr->meta_handle->numFds == 1 &&
                        media_ptr->meta_handle->numInts >= 2) {
 Input_pmem.fd = media_ptr->meta_handle->data[0];
 Input_pmem.buffer = media_ptr;
 Input_pmem.size = media_ptr->meta_handle->data[2];
 Input_pmem.offset = media_ptr->meta_handle->data[1];
                    DEBUG_PRINT_LOW("EBD fd = %d, offset = %d, size = %d",Input_pmem.fd,
 Input_pmem.offset,
 Input_pmem.size);
 } else if (media_ptr->buffer_type == kMetadataBufferTypeGrallocSource) {
 private_handle_t *handle = (private_handle_t *)media_ptr->meta_handle;
 Input_pmem.buffer = media_ptr;
 Input_pmem.fd = handle->fd;
 Input_pmem.offset = 0;
 Input_pmem.size = handle->size;
 } else {
                    meta_error = true;
                    DEBUG_PRINT_ERROR(" Meta Error set in EBD");
 }
 if (!meta_error)
                    meta_error = !dev_free_buf(&Input_pmem,PORT_INDEX_IN);
 if (meta_error) {
                    DEBUG_PRINT_ERROR(" Warning dev_free_buf failed flush value is %d",
                            input_flush_progress);
 }
 }
 }
 }
}
