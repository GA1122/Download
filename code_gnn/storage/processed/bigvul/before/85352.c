static void __submit_discard_cmd(struct f2fs_sb_info *sbi,
				struct discard_cmd *dc)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	struct bio *bio = NULL;

	if (dc->state != D_PREP)
		return;

	trace_f2fs_issue_discard(dc->bdev, dc->start, dc->len);

	dc->error = __blkdev_issue_discard(dc->bdev,
				SECTOR_FROM_BLOCK(dc->start),
				SECTOR_FROM_BLOCK(dc->len),
				GFP_NOFS, 0, &bio);
	if (!dc->error) {
		 
		dc->state = D_SUBMIT;
		atomic_inc(&dcc->issued_discard);
		atomic_inc(&dcc->issing_discard);
		if (bio) {
			bio->bi_private = dc;
			bio->bi_end_io = f2fs_submit_discard_endio;
			bio->bi_opf |= REQ_SYNC;
			submit_bio(bio);
			list_move_tail(&dc->list, &dcc->wait_list);
		}
	} else {
		__remove_discard_cmd(sbi, dc);
	}
}