void omx_vdec::free_extradata()
{
#ifdef USE_ION
 if (drv_ctx.extradata_info.uaddr) {
        munmap((void *)drv_ctx.extradata_info.uaddr, drv_ctx.extradata_info.size);
        close(drv_ctx.extradata_info.ion.fd_ion_data.fd);
        free_ion_memory(&drv_ctx.extradata_info.ion);
 }
    memset(&drv_ctx.extradata_info, 0, sizeof(drv_ctx.extradata_info));
#endif
 if (m_other_extradata) {
        free(m_other_extradata);
        m_other_extradata = NULL;
 }
}
