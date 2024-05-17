int vb2_ioctl_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p)
{
	struct video_device *vdev = video_devdata(file);
	int res = vb2_verify_memory_type(vdev->queue, p->memory, p->type);

	if (res)
		return res;
	if (vb2_queue_is_busy(vdev, file))
		return -EBUSY;
	res = vb2_core_reqbufs(vdev->queue, p->memory, &p->count);
	 
	if (res == 0)
		vdev->queue->owner = p->count ? file->private_data : NULL;
	return res;
}
