void omx_vdec::free_ion_memory(struct vdec_ion *buf_ion_info)
{

 if (!buf_ion_info) {
        DEBUG_PRINT_ERROR("ION: free called with invalid fd/allocdata");
 return;
 }
 if (ioctl(buf_ion_info->ion_device_fd,ION_IOC_FREE,
 &buf_ion_info->ion_alloc_data.handle)) {
        DEBUG_PRINT_ERROR("ION: free failed" );
 }
    close(buf_ion_info->ion_device_fd);
    buf_ion_info->ion_device_fd = -1;
    buf_ion_info->ion_alloc_data.handle = 0;
    buf_ion_info->fd_ion_data.fd = -1;
}
