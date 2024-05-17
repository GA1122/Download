static void __bind_mempools(struct mapped_device *md, struct dm_table *t)
{
	struct dm_md_mempools *p = dm_table_get_md_mempools(t);

	if (md->bs) {
		 
		if (dm_table_bio_based(t)) {
			 
			bioset_free(md->bs);
			md->bs = p->bs;
			p->bs = NULL;
		}
		 
		goto out;
	}

	BUG_ON(!p || md->io_pool || md->bs);

	md->io_pool = p->io_pool;
	p->io_pool = NULL;
	md->bs = p->bs;
	p->bs = NULL;

out:
	 
	dm_table_free_md_mempools(t);
}
