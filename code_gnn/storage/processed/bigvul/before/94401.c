static void sd_read_block_characteristics(struct scsi_disk *sdkp)
{
	unsigned char *buffer;
	u16 rot;
	const int vpd_len = 64;

	buffer = kmalloc(vpd_len, GFP_KERNEL);

	if (!buffer ||
	     
	    scsi_get_vpd_page(sdkp->device, 0xb1, buffer, vpd_len))
		goto out;

	rot = get_unaligned_be16(&buffer[4]);

	if (rot == 1)
		queue_flag_set_unlocked(QUEUE_FLAG_NONROT, sdkp->disk->queue);

 out:
	kfree(buffer);
}