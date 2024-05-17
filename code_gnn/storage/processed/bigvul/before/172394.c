bool omx_venc::dev_get_seq_hdr(void *buffer, unsigned size, unsigned *hdrlen)
{
 return handle->venc_get_seq_hdr(buffer, size, hdrlen);
}
