static void *vnc_worker_thread(void *arg)
{
    VncJobQueue *queue = arg;

    qemu_thread_get_self(&queue->thread);

    while (!vnc_worker_thread_loop(queue)) ;
    vnc_queue_clear(queue);
    return NULL;
}
