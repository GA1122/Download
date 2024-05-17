OMX_ERRORTYPE venc_dev::allocate_extradata()
{
 if (extradata_info.allocated) {
        DEBUG_PRINT_ERROR("Extradata already allocated!");
 return OMX_ErrorNone;
 }

#ifdef USE_ION

 if (extradata_info.buffer_size) {
 if (extradata_info.ion.ion_alloc_data.handle) {
            munmap((void *)extradata_info.uaddr, extradata_info.size);
            close(extradata_info.ion.fd_ion_data.fd);
            venc_handle->free_ion_memory(&extradata_info.ion);
 }

        extradata_info.size = ALIGN(extradata_info.size, SZ_4K);

        extradata_info.ion.ion_device_fd = venc_handle->alloc_map_ion_memory(
                extradata_info.size,
 &extradata_info.ion.ion_alloc_data,
 &extradata_info.ion.fd_ion_data, 0);

 if (extradata_info.ion.ion_device_fd < 0) {
            DEBUG_PRINT_ERROR("Failed to alloc extradata memory");
 return OMX_ErrorInsufficientResources;
 }

        extradata_info.uaddr = (char *)mmap(NULL,
                extradata_info.size,
                PROT_READ|PROT_WRITE, MAP_SHARED,
                extradata_info.ion.fd_ion_data.fd , 0);

 if (extradata_info.uaddr == MAP_FAILED) {
            DEBUG_PRINT_ERROR("Failed to map extradata memory");
            close(extradata_info.ion.fd_ion_data.fd);
            venc_handle->free_ion_memory(&extradata_info.ion);
 return OMX_ErrorInsufficientResources;
 }
 }

#endif
    extradata_info.allocated = 1;
 return OMX_ErrorNone;
}
