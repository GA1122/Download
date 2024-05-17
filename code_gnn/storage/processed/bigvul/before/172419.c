void venc_dev::free_extradata()
{
#ifdef USE_ION

 if (extradata_info.uaddr) {
        munmap((void *)extradata_info.uaddr, extradata_info.size);
        close(extradata_info.ion.fd_ion_data.fd);
        venc_handle->free_ion_memory(&extradata_info.ion);
 }

    memset(&extradata_info, 0, sizeof(extradata_info));
#endif
}
