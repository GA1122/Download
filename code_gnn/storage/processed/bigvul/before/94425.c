sd_store_cache_type(struct device *dev, struct device_attribute *attr,
		    const char *buf, size_t count)
{
	int i, ct = -1, rcd, wce, sp;
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct scsi_device *sdp = sdkp->device;
	char buffer[64];
	char *buffer_data;
	struct scsi_mode_data data;
	struct scsi_sense_hdr sshdr;
	int len;

	if (sdp->type != TYPE_DISK)
		 
		return -EINVAL;

	for (i = 0; i < ARRAY_SIZE(sd_cache_types); i++) {
		len = strlen(sd_cache_types[i]);
		if (strncmp(sd_cache_types[i], buf, len) == 0 &&
		    buf[len] == '\n') {
			ct = i;
			break;
		}
	}
	if (ct < 0)
		return -EINVAL;
	rcd = ct & 0x01 ? 1 : 0;
	wce = ct & 0x02 ? 1 : 0;
	if (scsi_mode_sense(sdp, 0x08, 8, buffer, sizeof(buffer), SD_TIMEOUT,
			    SD_MAX_RETRIES, &data, NULL))
		return -EINVAL;
	len = min_t(size_t, sizeof(buffer), data.length - data.header_length -
		  data.block_descriptor_length);
	buffer_data = buffer + data.header_length +
		data.block_descriptor_length;
	buffer_data[2] &= ~0x05;
	buffer_data[2] |= wce << 2 | rcd;
	sp = buffer_data[0] & 0x80 ? 1 : 0;

	if (scsi_mode_select(sdp, 1, sp, 8, buffer_data, len, SD_TIMEOUT,
			     SD_MAX_RETRIES, &data, &sshdr)) {
		if (scsi_sense_valid(&sshdr))
			sd_print_sense_hdr(sdkp, &sshdr);
		return -EINVAL;
	}
	revalidate_disk(sdkp->disk);
	return count;
}