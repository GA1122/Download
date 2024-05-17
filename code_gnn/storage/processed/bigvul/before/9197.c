static bool virtio_device_endian_needed(void *opaque)
{
    VirtIODevice *vdev = opaque;

    assert(vdev->device_endian != VIRTIO_DEVICE_ENDIAN_UNKNOWN);
    if (!virtio_vdev_has_feature(vdev, VIRTIO_F_VERSION_1)) {
        return vdev->device_endian != virtio_default_endian();
    }
     
    return vdev->device_endian != VIRTIO_DEVICE_ENDIAN_LITTLE;
}