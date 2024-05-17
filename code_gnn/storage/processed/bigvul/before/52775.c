static int vb2_queue_or_prepare_buf(struct vb2_queue *q, struct v4l2_buffer *b,
				    const char *opname)
{
	if (b->type != q->type) {
		dprintk(1, "%s: invalid buffer type\n", opname);
		return -EINVAL;
	}

	if (b->index >= q->num_buffers) {
		dprintk(1, "%s: buffer index out of range\n", opname);
		return -EINVAL;
	}

	if (q->bufs[b->index] == NULL) {
		 
		dprintk(1, "%s: buffer is NULL\n", opname);
		return -EINVAL;
	}

	if (b->memory != q->memory) {
		dprintk(1, "%s: invalid memory type\n", opname);
		return -EINVAL;
	}

	return __verify_planes_array(q->bufs[b->index], b);
}