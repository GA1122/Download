unsigned int vb2_fop_poll(struct file *file, poll_table *wait)
{
	struct video_device *vdev = video_devdata(file);
	struct vb2_queue *q = vdev->queue;
	struct mutex *lock = q->lock ? q->lock : vdev->lock;
	unsigned res;
	void *fileio;

	 
	WARN_ON(!lock);

	if (lock && mutex_lock_interruptible(lock))
		return POLLERR;

	fileio = q->fileio;

	res = vb2_poll(vdev->queue, file, wait);

	 
	if (!fileio && q->fileio)
		q->owner = file->private_data;
	if (lock)
		mutex_unlock(lock);
	return res;
}
