int btsock_thread_create(btsock_signaled_cb callback, btsock_cmd_cb cmd_callback)
{
    asrt(callback || cmd_callback);
    pthread_mutex_lock(&thread_slot_lock);
 int h = alloc_thread_slot();
    pthread_mutex_unlock(&thread_slot_lock);
    APPL_TRACE_DEBUG("alloc_thread_slot ret:%d", h);
 if(h >= 0)
 {
        init_poll(h);
 pthread_t thread;
 int status = create_thread(sock_poll_thread, (void*)(uintptr_t)h, &thread);
 if (status)
 {
            APPL_TRACE_ERROR("create_thread failed: %s", strerror(status));
            free_thread_slot(h);
 return -1;
 }

        ts[h].thread_id = thread;
        APPL_TRACE_DEBUG("h:%d, thread id:%d", h, ts[h].thread_id);
        ts[h].callback = callback;
        ts[h].cmd_callback = cmd_callback;
 }
 return h;
}
