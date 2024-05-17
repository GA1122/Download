void virtqueue_flush(VirtQueue *vq, unsigned int count)
{
    uint16_t old, new;
     
    smp_wmb();
    trace_virtqueue_flush(vq, count);
    old = vq->used_idx;
    new = old + count;
    vring_used_idx_set(vq, new);
    vq->inuse -= count;
    if (unlikely((int16_t)(new - vq->signalled_used) < (uint16_t)(new - old)))
        vq->signalled_used_valid = false;
}