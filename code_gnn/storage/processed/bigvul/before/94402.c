static void sd_read_block_limits(struct scsi_disk *sdkp)
{
	unsigned int sector_sz = sdkp->device->sector_size;
	const int vpd_len = 64;
	unsigned char *buffer = kmalloc(vpd_len, GFP_KERNEL);

	if (!buffer ||
	     
	    scsi_get_vpd_page(sdkp->device, 0xb0, buffer, vpd_len))
		goto out;

	blk_queue_io_min(sdkp->disk->queue,
			 get_unaligned_be16(&buffer[6]) * sector_sz);
	blk_queue_io_opt(sdkp->disk->queue,
			 get_unaligned_be32(&buffer[12]) * sector_sz);

	if (buffer[3] == 0x3c) {
		unsigned int lba_count, desc_count;

		sdkp->max_ws_blocks =
			(u32) min_not_zero(get_unaligned_be64(&buffer[36]),
					   (u64)0xffffffff);

		if (!sdkp->lbpme)
			goto out;

		lba_count = get_unaligned_be32(&buffer[20]);
		desc_count = get_unaligned_be32(&buffer[24]);

		if (lba_count && desc_count)
			sdkp->max_unmap_blocks = lba_count;

		sdkp->unmap_granularity = get_unaligned_be32(&buffer[28]);

		if (buffer[32] & 0x80)
			sdkp->unmap_alignment =
				get_unaligned_be32(&buffer[32]) & ~(1 << 31);

		if (!sdkp->lbpvpd) {  

			if (sdkp->max_unmap_blocks)
				sd_config_discard(sdkp, SD_LBP_UNMAP);
			else
				sd_config_discard(sdkp, SD_LBP_WS16);

		} else {	 

			if (sdkp->lbpu && sdkp->max_unmap_blocks)
				sd_config_discard(sdkp, SD_LBP_UNMAP);
			else if (sdkp->lbpws)
				sd_config_discard(sdkp, SD_LBP_WS16);
			else if (sdkp->lbpws10)
				sd_config_discard(sdkp, SD_LBP_WS10);
			else
				sd_config_discard(sdkp, SD_LBP_DISABLE);
		}
	}

 out:
	kfree(buffer);
}