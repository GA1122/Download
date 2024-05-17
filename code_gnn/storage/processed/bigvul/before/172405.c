OMX_U32 omx_venc::dev_set_message_thread_id(pthread_t tid)
{
 return handle->venc_set_message_thread_id(tid);
}
