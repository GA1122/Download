int aac_close_fib_context(struct aac_dev * dev, struct aac_fib_context * fibctx)
{
	struct fib *fib;

	 
	while (!list_empty(&fibctx->fib_list)) {
		struct list_head * entry;
		 
		entry = fibctx->fib_list.next;
		list_del(entry);
		fib = list_entry(entry, struct fib, fiblink);
		fibctx->count--;
		 
		kfree(fib->hw_fib_va);
		kfree(fib);
	}
	 
	list_del(&fibctx->next);
	 
	fibctx->type = 0;
	 
	kfree(fibctx);
	return 0;
}
