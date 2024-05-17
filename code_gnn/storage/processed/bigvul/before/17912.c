void vnc_start_worker_thread(void)
{
    VncJobQueue *q;

    if (vnc_worker_thread_running())
        return ;

    q = vnc_queue_init();
    qemu_thread_create(&q->thread, vnc_worker_thread, q, QEMU_THREAD_DETACHED);
    queue = q;  
}
