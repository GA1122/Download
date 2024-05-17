bool omx_venc::dev_empty_buf(void *buffer, void *pmem_data_buf,unsigned index,unsigned fd)
{
 bool bret = false;
    bret = handle->venc_empty_buf(buffer, pmem_data_buf,index,fd);
    hw_overload = handle->hw_overload;
 return bret;
}
