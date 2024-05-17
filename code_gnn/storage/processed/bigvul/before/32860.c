static struct thread_info *alloc_thread_info_node(struct task_struct *tsk,
						  int node)
{
	struct page *page = alloc_pages_node(node, THREADINFO_GFP_ACCOUNTED,
					     THREAD_SIZE_ORDER);

	return page ? page_address(page) : NULL;
}
