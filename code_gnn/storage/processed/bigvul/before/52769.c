unsigned int vb2_poll(struct vb2_queue *q, struct file *file, poll_table *wait)
{
	struct video_device *vfd = video_devdata(file);
	unsigned long req_events = poll_requested_events(wait);
	unsigned int res = 0;

	if (test_bit(V4L2_FL_USES_V4L2_FH, &vfd->flags)) {
		struct v4l2_fh *fh = file->private_data;

		if (v4l2_event_pending(fh))
			res = POLLPRI;
		else if (req_events & POLLPRI)
			poll_wait(file, &fh->wait, wait);
	}

	 
	if (q->waiting_for_buffers && (req_events & (POLLIN | POLLRDNORM)))
		return POLLERR;

	return res | vb2_core_poll(q, file, wait);
}