static void floppy_end_request(struct request *req, blk_status_t error)
{
	unsigned int nr_sectors = current_count_sectors;
	unsigned int drive = (unsigned long)req->rq_disk->private_data;

	 
	if (error)
		nr_sectors = blk_rq_cur_sectors(req);
	if (blk_update_request(req, error, nr_sectors << 9))
		return;
	__blk_mq_end_request(req, error);

	 
	floppy_off(drive);
	current_req = NULL;
}
