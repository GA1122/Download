static void vnc_lock_queue(VncJobQueue *queue)
{
    qemu_mutex_lock(&queue->mutex);
}
