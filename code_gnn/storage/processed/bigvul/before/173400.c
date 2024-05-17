OMX_ERRORTYPE omx_vdec::allocate_extradata()
{
#ifdef USE_ION
 if (drv_ctx.extradata_info.buffer_size) {
 if (drv_ctx.extradata_info.ion.ion_alloc_data.handle) {
            munmap((void *)drv_ctx.extradata_info.uaddr, drv_ctx.extradata_info.size);
            close(drv_ctx.extradata_info.ion.fd_ion_data.fd);
            free_ion_memory(&drv_ctx.extradata_info.ion);
 }
        drv_ctx.extradata_info.size = (drv_ctx.extradata_info.size + 4095) & (~4095);
        drv_ctx.extradata_info.ion.ion_device_fd = alloc_map_ion_memory(
                drv_ctx.extradata_info.size, 4096,
 &drv_ctx.extradata_info.ion.ion_alloc_data,
 &drv_ctx.extradata_info.ion.fd_ion_data, 0);
 if (drv_ctx.extradata_info.ion.ion_device_fd < 0) {
            DEBUG_PRINT_ERROR("Failed to alloc extradata memory");
 return OMX_ErrorInsufficientResources;
 }
        drv_ctx.extradata_info.uaddr = (char *)mmap(NULL,
                drv_ctx.extradata_info.size,
                PROT_READ|PROT_WRITE, MAP_SHARED,
                drv_ctx.extradata_info.ion.fd_ion_data.fd , 0);
 if (drv_ctx.extradata_info.uaddr == MAP_FAILED) {
            DEBUG_PRINT_ERROR("Failed to map extradata memory");
            close(drv_ctx.extradata_info.ion.fd_ion_data.fd);
            free_ion_memory(&drv_ctx.extradata_info.ion);
 return OMX_ErrorInsufficientResources;
 }
 }
#endif
 if (!m_other_extradata) {
        m_other_extradata = (OMX_OTHER_EXTRADATATYPE *)malloc(drv_ctx.extradata_info.buffer_size);
 if (!m_other_extradata) {
            DEBUG_PRINT_ERROR("Failed to alloc memory\n");
 return OMX_ErrorInsufficientResources;
 }
 }
 return OMX_ErrorNone;
}
