static void hugepage_subpool_put_pages(struct hugepage_subpool *spool,
				       long delta)
{
	if (!spool)
		return;

	spin_lock(&spool->lock);
	spool->used_hpages -= delta;
	 
	unlock_or_release_subpool(spool);
}
