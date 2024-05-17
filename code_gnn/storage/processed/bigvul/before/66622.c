static inline bool use_multiport(struct ports_device *portdev)
{
	 
	if (!portdev->vdev)
		return false;
	return __virtio_test_bit(portdev->vdev, VIRTIO_CONSOLE_F_MULTIPORT);
}
