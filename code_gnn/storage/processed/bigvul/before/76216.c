int cdrom_get_last_written(struct cdrom_device_info *cdi, long *last_written)
{
	struct cdrom_tocentry toc;
	disc_information di;
	track_information ti;
	__u32 last_track;
	int ret = -1, ti_size;

	if (!CDROM_CAN(CDC_GENERIC_PACKET))
		goto use_toc;

	ret = cdrom_get_disc_info(cdi, &di);
	if (ret < (int)(offsetof(typeof(di), last_track_lsb)
			+ sizeof(di.last_track_lsb)))
		goto use_toc;

	 
	last_track = (di.last_track_msb << 8) | di.last_track_lsb;
	ti_size = cdrom_get_track_info(cdi, last_track, 1, &ti);
	if (ti_size < (int)offsetof(typeof(ti), track_start))
		goto use_toc;

	 
	if (ti.blank) {
		if (last_track == 1)
			goto use_toc;
		last_track--;
		ti_size = cdrom_get_track_info(cdi, last_track, 1, &ti);
	}

	if (ti_size < (int)(offsetof(typeof(ti), track_size)
				+ sizeof(ti.track_size)))
		goto use_toc;

	 
	if (ti.lra_v && ti_size >= (int)(offsetof(typeof(ti), last_rec_address)
				+ sizeof(ti.last_rec_address))) {
		*last_written = be32_to_cpu(ti.last_rec_address);
	} else {
		 
		*last_written = be32_to_cpu(ti.track_start) +
				be32_to_cpu(ti.track_size);
		if (ti.free_blocks)
			*last_written -= (be32_to_cpu(ti.free_blocks) + 7);
	}
	return 0;

	 
use_toc:
	toc.cdte_format = CDROM_MSF;
	toc.cdte_track = CDROM_LEADOUT;
	if ((ret = cdi->ops->audio_ioctl(cdi, CDROMREADTOCENTRY, &toc)))
		return ret;
	sanitize_format(&toc.cdte_addr, &toc.cdte_format, CDROM_LBA);
	*last_written = toc.cdte_addr.lba;
	return 0;
}