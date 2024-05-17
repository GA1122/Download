static noinline int mmc_ioctl_cdrom_volume(struct cdrom_device_info *cdi,
					   void __user *arg,
					   struct packet_command *cgc,
					   unsigned int cmd)
{
	struct cdrom_volctrl volctrl;
	unsigned char buffer[32];
	char mask[sizeof(buffer)];
	unsigned short offset;
	int ret;

	cd_dbg(CD_DO_IOCTL, "entering CDROMVOLUME\n");

	if (copy_from_user(&volctrl, (struct cdrom_volctrl __user *)arg,
			   sizeof(volctrl)))
		return -EFAULT;

	cgc->buffer = buffer;
	cgc->buflen = 24;
	ret = cdrom_mode_sense(cdi, cgc, GPMODE_AUDIO_CTL_PAGE, 0);
	if (ret)
		return ret;
		
	 
	offset = 8 + be16_to_cpu(*(__be16 *)(buffer + 6));

	if (offset + 16 > sizeof(buffer))
		return -E2BIG;

	if (offset + 16 > cgc->buflen) {
		cgc->buflen = offset + 16;
		ret = cdrom_mode_sense(cdi, cgc,
				       GPMODE_AUDIO_CTL_PAGE, 0);
		if (ret)
			return ret;
	}

	 
	if ((buffer[offset] & 0x3f) != GPMODE_AUDIO_CTL_PAGE ||
	    buffer[offset + 1] < 14)
		return -EINVAL;

	 
	if (cmd == CDROMVOLREAD) {
		volctrl.channel0 = buffer[offset+9];
		volctrl.channel1 = buffer[offset+11];
		volctrl.channel2 = buffer[offset+13];
		volctrl.channel3 = buffer[offset+15];
		if (copy_to_user((struct cdrom_volctrl __user *)arg, &volctrl,
				 sizeof(volctrl)))
			return -EFAULT;
		return 0;
	}
		
	 
	cgc->buffer = mask;
	ret = cdrom_mode_sense(cdi, cgc, GPMODE_AUDIO_CTL_PAGE, 1);
	if (ret)
		return ret;

	buffer[offset + 9]  = volctrl.channel0 & mask[offset + 9];
	buffer[offset + 11] = volctrl.channel1 & mask[offset + 11];
	buffer[offset + 13] = volctrl.channel2 & mask[offset + 13];
	buffer[offset + 15] = volctrl.channel3 & mask[offset + 15];

	 
	cgc->buffer = buffer + offset - 8;
	memset(cgc->buffer, 0, 8);
	return cdrom_mode_select(cdi, cgc);
}
