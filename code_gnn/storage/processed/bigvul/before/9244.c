int virtio_set_features(VirtIODevice *vdev, uint64_t val)
{
    
    if (vdev->status & VIRTIO_CONFIG_S_FEATURES_OK) {
        return -EINVAL;
    }
    return virtio_set_features_nocheck(vdev, val);
}
