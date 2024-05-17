int sr_drive_status(struct cdrom_device_info *cdi, int slot)
{
	struct scsi_cd *cd = cdi->handle;
	struct scsi_sense_hdr sshdr;
	struct media_event_desc med;

	if (CDSL_CURRENT != slot) {
		 
		return -EINVAL;
	}
	if (!scsi_test_unit_ready(cd->device, SR_TIMEOUT, MAX_RETRIES, &sshdr))
		return CDS_DISC_OK;

	 
	if (scsi_sense_valid(&sshdr) && sshdr.sense_key == NOT_READY
			&& sshdr.asc == 0x04 && sshdr.ascq == 0x01)
		return CDS_DRIVE_NOT_READY;

	if (!cdrom_get_media_event(cdi, &med)) {
		if (med.media_present)
			return CDS_DISC_OK;
		else if (med.door_open)
			return CDS_TRAY_OPEN;
		else
			return CDS_NO_DISC;
	}

	 
	if (scsi_sense_valid(&sshdr) && sshdr.sense_key == NOT_READY
			&& sshdr.asc == 0x04 && sshdr.ascq == 0x02)
		return CDS_TRAY_OPEN;

	 
	if (sshdr.sense_key == NOT_READY && sshdr.asc == 0x04)
		return CDS_DISC_OK;

	 
	if (scsi_sense_valid(&sshdr) &&
	     
	    sshdr.asc == 0x3a)
		return CDS_NO_DISC;
	else
		return CDS_TRAY_OPEN;

	return CDS_DRIVE_NOT_READY;
}
