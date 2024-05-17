bool omx_vdec::align_pmem_buffers(int pmem_fd, OMX_U32 buffer_size,
        OMX_U32 alignment)
{
 struct pmem_allocation allocation;
    allocation.size = buffer_size;
    allocation.align = clip2(alignment);
 if (allocation.align < 4096) {
        allocation.align = 4096;
 }
 if (ioctl(pmem_fd, PMEM_ALLOCATE_ALIGNED, &allocation) < 0) {
        DEBUG_PRINT_ERROR("Aligment(%u) failed with pmem driver Sz(%lu)",
                allocation.align, allocation.size);
 return false;
 }
 return true;
}
