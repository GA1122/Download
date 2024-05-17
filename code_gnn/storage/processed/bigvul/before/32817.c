static long do_ioctl_trans(int fd, unsigned int cmd,
		 unsigned long arg, struct file *file)
{
	void __user *argp = compat_ptr(arg);

	switch (cmd) {
	case PPPIOCGIDLE32:
		return ppp_gidle(fd, cmd, argp);
	case PPPIOCSCOMPRESS32:
		return ppp_scompress(fd, cmd, argp);
	case PPPIOCSPASS32:
	case PPPIOCSACTIVE32:
		return ppp_sock_fprog_ioctl_trans(fd, cmd, argp);
#ifdef CONFIG_BLOCK
	case SG_IO:
		return sg_ioctl_trans(fd, cmd, argp);
	case SG_GET_REQUEST_TABLE:
		return sg_grt_trans(fd, cmd, argp);
	case MTIOCGET32:
	case MTIOCPOS32:
		return mt_ioctl_trans(fd, cmd, argp);
#endif
	 
	case TIOCGSERIAL:
	case TIOCSSERIAL:
		return serial_struct_ioctl(fd, cmd, argp);
	 
	case I2C_FUNCS:
		return w_long(fd, cmd, argp);
	case I2C_RDWR:
		return do_i2c_rdwr_ioctl(fd, cmd, argp);
	case I2C_SMBUS:
		return do_i2c_smbus_ioctl(fd, cmd, argp);
	 
	case RTC_IRQP_READ32:
	case RTC_IRQP_SET32:
	case RTC_EPOCH_READ32:
	case RTC_EPOCH_SET32:
		return rtc_ioctl(fd, cmd, argp);

	 
	case VIDEO_GET_EVENT:
		return do_video_get_event(fd, cmd, argp);
	case VIDEO_STILLPICTURE:
		return do_video_stillpicture(fd, cmd, argp);
	case VIDEO_SET_SPU_PALETTE:
		return do_video_set_spu_palette(fd, cmd, argp);
	}

	 
	switch (cmd) {
	 
	case TCSBRKP:
	case TIOCMIWAIT:
	case TIOCSCTTY:
	 
	case HOT_REMOVE_DISK:
	case HOT_ADD_DISK:
	case SET_DISK_FAULTY:
	case SET_BITMAP_FILE:
	 
	case KDSIGACCEPT:
	case KIOCSOUND:
	case KDMKTONE:
	case KDSETMODE:
	case KDSKBMODE:
	case KDSKBMETA:
	case KDSKBLED:
	case KDSETLED:
	 
	case NBD_SET_SOCK:
	case NBD_SET_BLKSIZE:
	case NBD_SET_SIZE:
	case NBD_SET_SIZE_BLOCKS:
		return do_vfs_ioctl(file, fd, cmd, arg);
	}

	return -ENOIOCTLCMD;
}
