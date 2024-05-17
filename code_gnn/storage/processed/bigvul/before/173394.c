int omx_vdec::alloc_map_ion_memory(OMX_U32 buffer_size,
        OMX_U32 alignment, struct ion_allocation_data *alloc_data,
 struct ion_fd_data *fd_data, int flag)
{
 int fd = -EINVAL;
 int rc = -EINVAL;
 int ion_dev_flag;
 struct vdec_ion ion_buf_info;
 if (!alloc_data || buffer_size <= 0 || !fd_data) {
        DEBUG_PRINT_ERROR("Invalid arguments to alloc_map_ion_memory");
 return -EINVAL;
 }
    ion_dev_flag = O_RDONLY;
    fd = open (MEM_DEVICE, ion_dev_flag);
 if (fd < 0) {
        DEBUG_PRINT_ERROR("opening ion device failed with fd = %d", fd);
 return fd;
 }
    alloc_data->flags = 0;
 if (!secure_mode && (flag & ION_FLAG_CACHED)) {
        alloc_data->flags |= ION_FLAG_CACHED;
 }
    alloc_data->len = buffer_size;
    alloc_data->align = clip2(alignment);
 if (alloc_data->align < 4096) {
        alloc_data->align = 4096;
 }
 if ((secure_mode) && (flag & ION_SECURE))
        alloc_data->flags |= ION_SECURE;

    alloc_data->ION_HEAP_MASK = ION_HEAP(ION_IOMMU_HEAP_ID);
 if (secure_mode && (alloc_data->flags & ION_SECURE))
        alloc_data->ION_HEAP_MASK = ION_HEAP(MEM_HEAP_ID);
    rc = ioctl(fd,ION_IOC_ALLOC,alloc_data);
 if (rc || !alloc_data->handle) {
        DEBUG_PRINT_ERROR("ION ALLOC memory failed");
        alloc_data->handle = 0;
        close(fd);
        fd = -ENOMEM;
 return fd;
 }
    fd_data->handle = alloc_data->handle;
    rc = ioctl(fd,ION_IOC_MAP,fd_data);
 if (rc) {
        DEBUG_PRINT_ERROR("ION MAP failed ");
        ion_buf_info.ion_alloc_data = *alloc_data;
        ion_buf_info.ion_device_fd = fd;
        ion_buf_info.fd_ion_data = *fd_data;
        free_ion_memory(&ion_buf_info);
        fd_data->fd =-1;
        fd = -ENOMEM;
 }

 return fd;
}
