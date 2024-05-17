bool omx_venc::dev_free_buf(void *buf_addr,unsigned port)
{
 return handle->venc_free_buf(buf_addr,port);
}
