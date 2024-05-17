static inline void btif_hh_close_poll_thread(btif_hh_device_t *p_dev)
{
    APPL_TRACE_DEBUG("%s", __FUNCTION__);
    p_dev->hh_keep_polling = 0;
 if(p_dev->hh_poll_thread_id > 0)
        pthread_join(p_dev->hh_poll_thread_id,NULL);

 return;
}
