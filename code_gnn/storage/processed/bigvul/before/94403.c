static void sd_read_block_provisioning(struct scsi_disk *sdkp)
{
	unsigned char *buffer;
	const int vpd_len = 8;

	if (sdkp->lbpme == 0)
		return;

	buffer = kmalloc(vpd_len, GFP_KERNEL);

	if (!buffer || scsi_get_vpd_page(sdkp->device, 0xb2, buffer, vpd_len))
		goto out;

	sdkp->lbpvpd	= 1;
	sdkp->lbpu	= (buffer[5] >> 7) & 1;	 
	sdkp->lbpws	= (buffer[5] >> 6) & 1;	 
	sdkp->lbpws10	= (buffer[5] >> 5) & 1;	 

 out:
	kfree(buffer);
}