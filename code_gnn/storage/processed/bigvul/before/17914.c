static void vnc_unlock_queue(VncJobQueue *queue)
{
    qemu_mutex_unlock(&queue->mutex);
}
