void virtio_queue_set_num(VirtIODevice *vdev, int n, int num)
{
     
    if (!!num != !!vdev->vq[n].vring.num ||
        num > VIRTQUEUE_MAX_SIZE ||
        num < 0) {
        return;
    }
    vdev->vq[n].vring.num = num;
    virtqueue_init(&vdev->vq[n]);
}
