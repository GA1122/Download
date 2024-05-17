void virtio_queue_set_host_notifier_fd_handler(VirtQueue *vq, bool assign,
                                               bool set_handler)
{
    if (assign && set_handler) {
        event_notifier_set_handler(&vq->host_notifier,
                                   virtio_queue_host_notifier_read);
    } else {
        event_notifier_set_handler(&vq->host_notifier, NULL);
    }
    if (!assign) {
         
        virtio_queue_host_notifier_read(&vq->host_notifier);
    }
}
