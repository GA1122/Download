static void persistent_dtr(struct dm_exception_store *store)
{
	struct pstore *ps = get_info(store);

	destroy_workqueue(ps->metadata_wq);

	 
	if (ps->io_client)
		dm_io_client_destroy(ps->io_client);
	free_area(ps);

	 
	if (ps->callbacks)
		vfree(ps->callbacks);

	kfree(ps);
}
