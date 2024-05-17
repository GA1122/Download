static void vnc_queue_clear(VncJobQueue *q)
{
    qemu_cond_destroy(&queue->cond);
    qemu_mutex_destroy(&queue->mutex);
    buffer_free(&queue->buffer);
    g_free(q);
    queue = NULL;  
}
