static void vhost_init_is_le(struct vhost_virtqueue *vq)
{
	 
	vq->is_le = vhost_has_feature(vq, VIRTIO_F_VERSION_1) || !vq->user_be;
}
