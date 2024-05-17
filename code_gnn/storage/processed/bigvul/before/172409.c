bool omx_venc::dev_use_buf(void *buf_addr,unsigned port,unsigned index)
{
 return handle->venc_use_buf(buf_addr,port,index);
}
