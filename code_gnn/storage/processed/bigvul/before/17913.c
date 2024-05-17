void vnc_stop_worker_thread(void)
{
    if (!vnc_worker_thread_running())
        return ;

     
    vnc_lock_queue(queue);
    queue->exit = true;
    vnc_unlock_queue(queue);
    vnc_jobs_clear(NULL);
    qemu_cond_broadcast(&queue->cond);
}
