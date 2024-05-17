void blk_account_io_start(struct request *rq, bool new_io)
{
	struct hd_struct *part;
	int rw = rq_data_dir(rq);
	int cpu;

	if (!blk_do_io_stat(rq))
		return;

	cpu = part_stat_lock();

	if (!new_io) {
		part = rq->part;
		part_stat_inc(cpu, part, merges[rw]);
	} else {
		part = disk_map_sector_rcu(rq->rq_disk, blk_rq_pos(rq));
		if (!hd_struct_try_get(part)) {
			 
			part = &rq->rq_disk->part0;
			hd_struct_get(part);
		}
		part_round_stats(rq->q, cpu, part);
		part_inc_in_flight(rq->q, part, rw);
		rq->part = part;
	}

	part_stat_unlock();
}
