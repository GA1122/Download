static int scsi_setup_discard_cmnd(struct scsi_device *sdp, struct request *rq)
{
	struct scsi_disk *sdkp = scsi_disk(rq->rq_disk);
	struct bio *bio = rq->bio;
	sector_t sector = bio->bi_sector;
	unsigned int nr_sectors = bio_sectors(bio);
	unsigned int len;
	int ret;
	char *buf;
	struct page *page;

	if (sdkp->device->sector_size == 4096) {
		sector >>= 3;
		nr_sectors >>= 3;
	}

	rq->timeout = SD_TIMEOUT;

	memset(rq->cmd, 0, rq->cmd_len);

	page = alloc_page(GFP_ATOMIC | __GFP_ZERO);
	if (!page)
		return BLKPREP_DEFER;

	switch (sdkp->provisioning_mode) {
	case SD_LBP_UNMAP:
		buf = page_address(page);

		rq->cmd_len = 10;
		rq->cmd[0] = UNMAP;
		rq->cmd[8] = 24;

		put_unaligned_be16(6 + 16, &buf[0]);
		put_unaligned_be16(16, &buf[2]);
		put_unaligned_be64(sector, &buf[8]);
		put_unaligned_be32(nr_sectors, &buf[16]);

		len = 24;
		break;

	case SD_LBP_WS16:
		rq->cmd_len = 16;
		rq->cmd[0] = WRITE_SAME_16;
		rq->cmd[1] = 0x8;  
		put_unaligned_be64(sector, &rq->cmd[2]);
		put_unaligned_be32(nr_sectors, &rq->cmd[10]);

		len = sdkp->device->sector_size;
		break;

	case SD_LBP_WS10:
	case SD_LBP_ZERO:
		rq->cmd_len = 10;
		rq->cmd[0] = WRITE_SAME;
		if (sdkp->provisioning_mode == SD_LBP_WS10)
			rq->cmd[1] = 0x8;  
		put_unaligned_be32(sector, &rq->cmd[2]);
		put_unaligned_be16(nr_sectors, &rq->cmd[7]);

		len = sdkp->device->sector_size;
		break;

	default:
		ret = BLKPREP_KILL;
		goto out;
	}

	blk_add_request_payload(rq, page, len);
	ret = scsi_setup_blk_pc_cmnd(sdp, rq);
	rq->buffer = page_address(page);

out:
	if (ret != BLKPREP_OK) {
		__free_page(page);
		rq->buffer = NULL;
	}
	return ret;
}