void virtio_queue_set_host_notifier_fd_handler(VirtQueue *vq, bool assign,
                                               bool set_handler)
{
    AioContext *ctx = qemu_get_aio_context();
    if (assign && set_handler) {
        if (vq->use_aio) {
            aio_set_event_notifier(ctx, &vq->host_notifier, true,
                                   virtio_queue_host_notifier_read);
        } else {
            event_notifier_set_handler(&vq->host_notifier, true,
                                       virtio_queue_host_notifier_read);
        }
    } else {
        if (vq->use_aio) {
            aio_set_event_notifier(ctx, &vq->host_notifier, true, NULL);
        } else {
            event_notifier_set_handler(&vq->host_notifier, true, NULL);
        }
    }
    if (!assign) {
         
        virtio_queue_host_notifier_read(&vq->host_notifier);
    }
}
