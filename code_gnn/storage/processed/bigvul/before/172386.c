bool omx_venc::dev_fill_buf(void *buffer, void *pmem_data_buf,unsigned index,unsigned fd)
{
 return handle->venc_fill_buf(buffer, pmem_data_buf,index,fd);
}
