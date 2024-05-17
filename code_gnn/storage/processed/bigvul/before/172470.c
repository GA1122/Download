unsigned venc_dev::venc_set_message_thread_id(pthread_t tid)
{
    async_thread_created = true;
    m_tid=tid;
 return 0;
}
