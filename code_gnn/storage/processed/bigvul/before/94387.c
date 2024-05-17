static unsigned int sd_completed_bytes(struct scsi_cmnd *scmd)
{
	u64 start_lba = blk_rq_pos(scmd->request);
	u64 end_lba = blk_rq_pos(scmd->request) + (scsi_bufflen(scmd) / 512);
	u64 bad_lba;
	int info_valid;
	 
	unsigned int transferred = scsi_bufflen(scmd) - scsi_get_resid(scmd);
	unsigned int good_bytes;

	if (scmd->request->cmd_type != REQ_TYPE_FS)
		return 0;

	info_valid = scsi_get_sense_info_fld(scmd->sense_buffer,
					     SCSI_SENSE_BUFFERSIZE,
					     &bad_lba);
	if (!info_valid)
		return 0;

	if (scsi_bufflen(scmd) <= scmd->device->sector_size)
		return 0;

	if (scmd->device->sector_size < 512) {
		 
		start_lba <<= 1;
		end_lba <<= 1;
	} else {
		 
		u64 factor = scmd->device->sector_size / 512;
		do_div(start_lba, factor);
		do_div(end_lba, factor);
	}

	 
	if (bad_lba < start_lba  || bad_lba >= end_lba)
		return 0;

	 
	good_bytes = (bad_lba - start_lba) * scmd->device->sector_size;
	return min(good_bytes, transferred);
}
