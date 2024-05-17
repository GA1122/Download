static int multipath_map(struct dm_target *ti, struct request *clone,
			 union map_info *map_context)
{
	int r;
	struct dm_mpath_io *mpio;
	struct multipath *m = (struct multipath *) ti->private;

	mpio = mempool_alloc(m->mpio_pool, GFP_ATOMIC);
	if (!mpio)
		 
		return DM_MAPIO_REQUEUE;
	memset(mpio, 0, sizeof(*mpio));

	map_context->ptr = mpio;
	clone->cmd_flags |= REQ_FAILFAST_TRANSPORT;
	r = map_io(m, clone, mpio, 0);
	if (r < 0 || r == DM_MAPIO_REQUEUE)
		mempool_free(mpio, m->mpio_pool);

	return r;
}