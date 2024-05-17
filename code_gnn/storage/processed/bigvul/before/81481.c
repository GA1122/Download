tracing_mark_write(struct file *filp, const char __user *ubuf,
					size_t cnt, loff_t *fpos)
{
	struct trace_array *tr = filp->private_data;
	struct ring_buffer_event *event;
	enum event_trigger_type tt = ETT_NONE;
	struct ring_buffer *buffer;
	struct print_entry *entry;
	unsigned long irq_flags;
	const char faulted[] = "<faulted>";
	ssize_t written;
	int size;
	int len;

 
#define FAULTED_SIZE (sizeof(faulted) - 1)  

	if (tracing_disabled)
		return -EINVAL;

	if (!(tr->trace_flags & TRACE_ITER_MARKERS))
		return -EINVAL;

	if (cnt > TRACE_BUF_SIZE)
		cnt = TRACE_BUF_SIZE;

	BUILD_BUG_ON(TRACE_BUF_SIZE >= PAGE_SIZE);

	local_save_flags(irq_flags);
	size = sizeof(*entry) + cnt + 2;  

	 
	if (cnt < FAULTED_SIZE)
		size += FAULTED_SIZE - cnt;

	buffer = tr->trace_buffer.buffer;
	event = __trace_buffer_lock_reserve(buffer, TRACE_PRINT, size,
					    irq_flags, preempt_count());
	if (unlikely(!event))
		 
		return -EBADF;

	entry = ring_buffer_event_data(event);
	entry->ip = _THIS_IP_;

	len = __copy_from_user_inatomic(&entry->buf, ubuf, cnt);
	if (len) {
		memcpy(&entry->buf, faulted, FAULTED_SIZE);
		cnt = FAULTED_SIZE;
		written = -EFAULT;
	} else
		written = cnt;
	len = cnt;

	if (tr->trace_marker_file && !list_empty(&tr->trace_marker_file->triggers)) {
		 
		entry->buf[cnt] = '\0';
		tt = event_triggers_call(tr->trace_marker_file, entry, event);
	}

	if (entry->buf[cnt - 1] != '\n') {
		entry->buf[cnt] = '\n';
		entry->buf[cnt + 1] = '\0';
	} else
		entry->buf[cnt] = '\0';

	__buffer_unlock_commit(buffer, event);

	if (tt)
		event_triggers_post_call(tr->trace_marker_file, tt);

	if (written > 0)
		*fpos += written;

	return written;
}