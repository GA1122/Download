static void userfaultfd_show_fdinfo(struct seq_file *m, struct file *f)
{
	struct userfaultfd_ctx *ctx = f->private_data;
	wait_queue_entry_t *wq;
	unsigned long pending = 0, total = 0;

	spin_lock(&ctx->fault_pending_wqh.lock);
	list_for_each_entry(wq, &ctx->fault_pending_wqh.head, entry) {
		pending++;
		total++;
	}
	list_for_each_entry(wq, &ctx->fault_wqh.head, entry) {
		total++;
	}
	spin_unlock(&ctx->fault_pending_wqh.lock);

	 
	seq_printf(m, "pending:\t%lu\ntotal:\t%lu\nAPI:\t%Lx:%x:%Lx\n",
		   pending, total, UFFD_API, ctx->features,
		   UFFD_API_IOCTLS|UFFD_API_RANGE_IOCTLS);
}