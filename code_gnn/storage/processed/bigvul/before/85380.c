static void do_write_page(struct f2fs_summary *sum, struct f2fs_io_info *fio)
{
	int type = __get_segment_type(fio);
	int err;

reallocate:
	allocate_data_block(fio->sbi, fio->page, fio->old_blkaddr,
			&fio->new_blkaddr, sum, type, fio, true);

	 
	err = f2fs_submit_page_write(fio);
	if (err == -EAGAIN) {
		fio->old_blkaddr = fio->new_blkaddr;
		goto reallocate;
	}
}