void virtqueue_discard(VirtQueue *vq, const VirtQueueElement *elem,
                       unsigned int len)
{
    vq->last_avail_idx--;
    vq->inuse--;
    virtqueue_unmap_sg(vq, elem, len);
}
