void rds_rdma_free_op(struct rm_rdma_op *ro)
{
	unsigned int i;

	for (i = 0; i < ro->op_nents; i++) {
		struct page *page = sg_page(&ro->op_sg[i]);

		 
		if (!ro->op_write) {
			WARN_ON(!page->mapping && irqs_disabled());
			set_page_dirty(page);
		}
		put_page(page);
	}

	kfree(ro->op_notifier);
	ro->op_notifier = NULL;
	ro->op_active = 0;
}
