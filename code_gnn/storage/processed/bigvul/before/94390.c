static int sd_done(struct scsi_cmnd *SCpnt)
{
	int result = SCpnt->result;
	unsigned int good_bytes = result ? 0 : scsi_bufflen(SCpnt);
	struct scsi_sense_hdr sshdr;
	struct scsi_disk *sdkp = scsi_disk(SCpnt->request->rq_disk);
	int sense_valid = 0;
	int sense_deferred = 0;
	unsigned char op = SCpnt->cmnd[0];

	if ((SCpnt->request->cmd_flags & REQ_DISCARD) && !result)
		scsi_set_resid(SCpnt, 0);

	if (result) {
		sense_valid = scsi_command_normalize_sense(SCpnt, &sshdr);
		if (sense_valid)
			sense_deferred = scsi_sense_is_deferred(&sshdr);
	}
#ifdef CONFIG_SCSI_LOGGING
	SCSI_LOG_HLCOMPLETE(1, scsi_print_result(SCpnt));
	if (sense_valid) {
		SCSI_LOG_HLCOMPLETE(1, scmd_printk(KERN_INFO, SCpnt,
						   "sd_done: sb[respc,sk,asc,"
						   "ascq]=%x,%x,%x,%x\n",
						   sshdr.response_code,
						   sshdr.sense_key, sshdr.asc,
						   sshdr.ascq));
	}
#endif
	if (driver_byte(result) != DRIVER_SENSE &&
	    (!sense_valid || sense_deferred))
		goto out;

	switch (sshdr.sense_key) {
	case HARDWARE_ERROR:
	case MEDIUM_ERROR:
		good_bytes = sd_completed_bytes(SCpnt);
		break;
	case RECOVERED_ERROR:
		good_bytes = scsi_bufflen(SCpnt);
		break;
	case NO_SENSE:
		 
		scsi_print_sense("sd", SCpnt);
		SCpnt->result = 0;
		memset(SCpnt->sense_buffer, 0, SCSI_SENSE_BUFFERSIZE);
		break;
	case ABORTED_COMMAND:
		if (sshdr.asc == 0x10)   
			good_bytes = sd_completed_bytes(SCpnt);
		break;
	case ILLEGAL_REQUEST:
		if (sshdr.asc == 0x10)   
			good_bytes = sd_completed_bytes(SCpnt);
		 
		if ((sshdr.asc == 0x20 || sshdr.asc == 0x24) &&
		    (op == UNMAP || op == WRITE_SAME_16 || op == WRITE_SAME))
			sd_config_discard(sdkp, SD_LBP_DISABLE);
		break;
	default:
		break;
	}
 out:
	if (rq_data_dir(SCpnt->request) == READ && scsi_prot_sg_count(SCpnt))
		sd_dif_complete(SCpnt, good_bytes);

	if (scsi_host_dif_capable(sdkp->device->host, sdkp->protection_type)
	    == SD_DIF_TYPE2_PROTECTION && SCpnt->cmnd != SCpnt->request->cmd) {

		 
		if (result)
			scsi_print_command(SCpnt);

		mempool_free(SCpnt->cmnd, sd_cdb_pool);
		SCpnt->cmnd = NULL;
		SCpnt->cmd_len = 0;
	}

	return good_bytes;
}