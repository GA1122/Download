static int __videobuf_copy_to_user ( struct videobuf_queue *q,
				char __user *data, size_t count,
				int nonblocking )
{
	struct videbuf_vmalloc_memory *mem=q->read_buf->priv;
	BUG_ON (!mem);
	MAGIC_CHECK(mem->magic,MAGIC_VMAL_MEM);

	BUG_ON (!mem->vmalloc);

	 
	if (count > q->read_buf->size - q->read_off)
		count = q->read_buf->size - q->read_off;

	if (copy_to_user(data, mem->vmalloc+q->read_off, count))
		return -EFAULT;

	return count;
}