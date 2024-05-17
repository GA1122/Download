static int sd_try_extended_inquiry(struct scsi_device *sdp)
{
	 
	if (sdp->scsi_level > SCSI_SPC_2)
		return 1;
	return 0;
}
