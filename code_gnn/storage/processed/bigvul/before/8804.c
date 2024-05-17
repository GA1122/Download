static void iov_buffer_upload(void *cookie, uint32_t doff, void *src, int len)
{
   struct virgl_sub_upload_data *d = cookie;
   glBufferSubData(d->target, d->box->x + doff, len, src);
}
