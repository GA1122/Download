void rds_atomic_free_op(struct rm_atomic_op *ao)
{
	struct page *page = sg_page(ao->op_sg);

	 
	set_page_dirty(page);
	put_page(page);

	kfree(ao->op_notifier);
	ao->op_notifier = NULL;
	ao->op_active = 0;
}
