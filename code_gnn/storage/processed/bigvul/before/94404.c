sd_read_cache_type(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int len = 0, res;
	struct scsi_device *sdp = sdkp->device;

	int dbd;
	int modepage;
	int first_len;
	struct scsi_mode_data data;
	struct scsi_sense_hdr sshdr;
	int old_wce = sdkp->WCE;
	int old_rcd = sdkp->RCD;
	int old_dpofua = sdkp->DPOFUA;

	first_len = 4;
	if (sdp->skip_ms_page_8) {
		if (sdp->type == TYPE_RBC)
			goto defaults;
		else {
			if (sdp->skip_ms_page_3f)
				goto defaults;
			modepage = 0x3F;
			if (sdp->use_192_bytes_for_3f)
				first_len = 192;
			dbd = 0;
		}
	} else if (sdp->type == TYPE_RBC) {
		modepage = 6;
		dbd = 8;
	} else {
		modepage = 8;
		dbd = 0;
	}

	 
	res = sd_do_mode_sense(sdp, dbd, modepage, buffer, first_len,
			&data, &sshdr);

	if (!scsi_status_is_good(res))
		goto bad_sense;

	if (!data.header_length) {
		modepage = 6;
		first_len = 0;
		sd_printk(KERN_ERR, sdkp, "Missing header in MODE_SENSE response\n");
	}

	 
	len = data.length;

	 
	if (len < 3)
		goto bad_sense;
	else if (len > SD_BUF_SIZE) {
		sd_printk(KERN_NOTICE, sdkp, "Truncating mode parameter "
			  "data from %d to %d bytes\n", len, SD_BUF_SIZE);
		len = SD_BUF_SIZE;
	}
	if (modepage == 0x3F && sdp->use_192_bytes_for_3f)
		len = 192;

	 
	if (len > first_len)
		res = sd_do_mode_sense(sdp, dbd, modepage, buffer, len,
				&data, &sshdr);

	if (scsi_status_is_good(res)) {
		int offset = data.header_length + data.block_descriptor_length;

		while (offset < len) {
			u8 page_code = buffer[offset] & 0x3F;
			u8 spf       = buffer[offset] & 0x40;

			if (page_code == 8 || page_code == 6) {
				 
				if (len - offset <= 2) {
					sd_printk(KERN_ERR, sdkp, "Incomplete "
						  "mode parameter data\n");
					goto defaults;
				} else {
					modepage = page_code;
					goto Page_found;
				}
			} else {
				 
				if (spf && len - offset > 3)
					offset += 4 + (buffer[offset+2] << 8) +
						buffer[offset+3];
				else if (!spf && len - offset > 1)
					offset += 2 + buffer[offset+1];
				else {
					sd_printk(KERN_ERR, sdkp, "Incomplete "
						  "mode parameter data\n");
					goto defaults;
				}
			}
		}

		if (modepage == 0x3F) {
			sd_printk(KERN_ERR, sdkp, "No Caching mode page "
				  "present\n");
			goto defaults;
		} else if ((buffer[offset] & 0x3f) != modepage) {
			sd_printk(KERN_ERR, sdkp, "Got wrong page\n");
			goto defaults;
		}
	Page_found:
		if (modepage == 8) {
			sdkp->WCE = ((buffer[offset + 2] & 0x04) != 0);
			sdkp->RCD = ((buffer[offset + 2] & 0x01) != 0);
		} else {
			sdkp->WCE = ((buffer[offset + 2] & 0x01) == 0);
			sdkp->RCD = 0;
		}

		sdkp->DPOFUA = (data.device_specific & 0x10) != 0;
		if (sdkp->DPOFUA && !sdkp->device->use_10_for_rw) {
			sd_printk(KERN_NOTICE, sdkp,
				  "Uses READ/WRITE(6), disabling FUA\n");
			sdkp->DPOFUA = 0;
		}

		if (sdkp->first_scan || old_wce != sdkp->WCE ||
		    old_rcd != sdkp->RCD || old_dpofua != sdkp->DPOFUA)
			sd_printk(KERN_NOTICE, sdkp,
				  "Write cache: %s, read cache: %s, %s\n",
				  sdkp->WCE ? "enabled" : "disabled",
				  sdkp->RCD ? "disabled" : "enabled",
				  sdkp->DPOFUA ? "supports DPO and FUA"
				  : "doesn't support DPO or FUA");

		return;
	}

bad_sense:
	if (scsi_sense_valid(&sshdr) &&
	    sshdr.sense_key == ILLEGAL_REQUEST &&
	    sshdr.asc == 0x24 && sshdr.ascq == 0x0)
		 
		sd_printk(KERN_NOTICE, sdkp, "Cache data unavailable\n");
	else
		sd_printk(KERN_ERR, sdkp, "Asking for cache data failed\n");

defaults:
	sd_printk(KERN_ERR, sdkp, "Assuming drive cache: write through\n");
	sdkp->WCE = 0;
	sdkp->RCD = 0;
	sdkp->DPOFUA = 0;
}