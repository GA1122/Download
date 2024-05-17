void virtio_queue_set_align(VirtIODevice *vdev, int n, int align)
{
    BusState *qbus = qdev_get_parent_bus(DEVICE(vdev));
    VirtioBusClass *k = VIRTIO_BUS_GET_CLASS(qbus);

     
    assert(k->has_variable_vring_alignment);

    vdev->vq[n].vring.align = align;
    virtqueue_init(&vdev->vq[n]);
}
