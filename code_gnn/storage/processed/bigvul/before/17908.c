void vnc_jobs_join(VncState *vs)
{
    vnc_lock_queue(queue);
    while (vnc_has_job_locked(vs)) {
        qemu_cond_wait(&queue->cond, &queue->mutex);
    }
    vnc_unlock_queue(queue);
    vnc_jobs_consume_buffer(vs);
}
