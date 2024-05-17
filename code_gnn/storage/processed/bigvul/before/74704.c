static int __videobuf_copy_stream ( struct videobuf_queue *q,
				char __user *data, size_t count, size_t pos,
				int vbihack, int nonblocking )
{
	unsigned int  *fc;
	struct videbuf_vmalloc_memory *mem=q->read_buf->priv;
	BUG_ON (!mem);
	MAGIC_CHECK(mem->magic,MAGIC_VMAL_MEM);

	if (vbihack) {
		 
		fc  = (unsigned int*)mem->vmalloc;
		fc += (q->read_buf->size>>2) -1;
		*fc = q->read_buf->field_count >> 1;
		dprintk(1,"vbihack: %d\n",*fc);
	}

	 
	count = __videobuf_copy_to_user (q,data,count,nonblocking);

	if ( (count==-EFAULT) && (0 == pos) )
		return -EFAULT;

	return count;
}