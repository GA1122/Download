int omx_video::alloc_map_ion_memory(int size,
 struct ion_allocation_data *alloc_data,
 struct ion_fd_data *fd_data,int flag)
{
 struct venc_ion buf_ion_info;
 int ion_device_fd =-1,rc=0,ion_dev_flags = 0;
 if (size <=0 || !alloc_data || !fd_data) {
        DEBUG_PRINT_ERROR("Invalid input to alloc_map_ion_memory");
 return -EINVAL;
 }

    ion_dev_flags = O_RDONLY;
    ion_device_fd = open (MEM_DEVICE,ion_dev_flags);
 if (ion_device_fd < 0) {
        DEBUG_PRINT_ERROR("ERROR: ION Device open() Failed");
 return ion_device_fd;
 }

 if(secure_session) {
        alloc_data->len = (size + (SZ_1M - 1)) & ~(SZ_1M - 1);
        alloc_data->align = SZ_1M;
        alloc_data->flags = ION_SECURE;
        alloc_data->ION_HEAP_MASK = ION_HEAP(ION_CP_MM_HEAP_ID);
        DEBUG_PRINT_HIGH("ION ALLOC sec buf: size %u align %u flags %x",
 (unsigned int)alloc_data->len, (unsigned int)alloc_data->align,
                alloc_data->flags);
 } else {
        alloc_data->len = (size + (SZ_4K - 1)) & ~(SZ_4K - 1);
        alloc_data->align = SZ_4K;
        alloc_data->flags = (flag & ION_FLAG_CACHED ? ION_FLAG_CACHED : 0);
#ifdef MAX_RES_720P
        alloc_data->ION_HEAP_MASK = ION_HEAP(MEM_HEAP_ID);
#else
        alloc_data->ION_HEAP_MASK = (ION_HEAP(MEM_HEAP_ID) |
            ION_HEAP(ION_IOMMU_HEAP_ID));
#endif
        DEBUG_PRINT_HIGH("ION ALLOC unsec buf: size %u align %u flags %x",
 (unsigned int)alloc_data->len, (unsigned int)alloc_data->align,
                alloc_data->flags);
 }

    rc = ioctl(ion_device_fd,ION_IOC_ALLOC,alloc_data);
 if (rc || !alloc_data->handle) {
        DEBUG_PRINT_ERROR("ION ALLOC memory failed 0x%x", rc);
        alloc_data->handle = 0;
        close(ion_device_fd);
        ion_device_fd = -1;
 return ion_device_fd;
 }
    fd_data->handle = alloc_data->handle;
    rc = ioctl(ion_device_fd,ION_IOC_MAP,fd_data);
 if (rc) {
        DEBUG_PRINT_ERROR("ION MAP failed ");
        buf_ion_info.ion_alloc_data = *alloc_data;
        buf_ion_info.ion_device_fd = ion_device_fd;
        buf_ion_info.fd_ion_data = *fd_data;
        free_ion_memory(&buf_ion_info);
        fd_data->fd =-1;
        ion_device_fd =-1;
 }
 return ion_device_fd;
}
