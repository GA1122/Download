static int chunk_io(struct pstore *ps, void *area, chunk_t chunk, int rw,
		    int metadata)
{
	struct dm_io_region where = {
		.bdev = dm_snap_cow(ps->store->snap)->bdev,
		.sector = ps->store->chunk_size * chunk,
		.count = ps->store->chunk_size,
	};
	struct dm_io_request io_req = {
		.bi_rw = rw,
		.mem.type = DM_IO_VMA,
		.mem.ptr.vma = area,
		.client = ps->io_client,
		.notify.fn = NULL,
	};
	struct mdata_req req;

	if (!metadata)
		return dm_io(&io_req, 1, &where, NULL);

	req.where = &where;
	req.io_req = &io_req;

	 
	INIT_WORK_ONSTACK(&req.work, do_metadata);
	queue_work(ps->metadata_wq, &req.work);
	flush_workqueue(ps->metadata_wq);

	return req.result;
}