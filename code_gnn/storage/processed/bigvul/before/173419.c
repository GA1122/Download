OMX_ERRORTYPE omx_vdec::allocate_color_convert_buf::cache_ops(
 unsigned int index, unsigned int cmd)
{
 if (!enabled) {
 return OMX_ErrorNone;
 }

 if (!omx || index >= omx->drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("%s: Invalid param", __func__);
 return OMX_ErrorBadParameter;
 }

 struct ion_flush_data flush_data;
 struct ion_custom_data custom_data;

    memset(&flush_data, 0x0, sizeof(flush_data));
    memset(&custom_data, 0x0, sizeof(custom_data));

    flush_data.vaddr = pmem_baseaddress[index];
    flush_data.fd = op_buf_ion_info[index].fd_ion_data.fd;
    flush_data.handle = op_buf_ion_info[index].fd_ion_data.handle;
    flush_data.length = buffer_size_req;
    custom_data.cmd = cmd;
    custom_data.arg = (unsigned long)&flush_data;

    DEBUG_PRINT_LOW("Cache %s: fd=%d handle=%d va=%p size=%d",
 (cmd == ION_IOC_CLEAN_CACHES) ? "Clean" : "Invalidate",
            flush_data.fd, flush_data.handle, flush_data.vaddr,
            flush_data.length);
 int ret = ioctl(op_buf_ion_info[index].ion_device_fd, ION_IOC_CUSTOM, &custom_data);
 if (ret < 0) {
        DEBUG_PRINT_ERROR("Cache %s failed: %s\n",
 (cmd == ION_IOC_CLEAN_CACHES) ? "Clean" : "Invalidate",
                strerror(errno));
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
