static int cdrom_get_disc_info(struct cdrom_device_info *cdi,
			       disc_information *di)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	struct packet_command cgc;
	int ret, buflen;

	 
	init_cdrom_command(&cgc, di, sizeof(*di), CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_READ_DISC_INFO;
	cgc.cmd[8] = cgc.buflen = 2;
	cgc.quiet = 1;

	ret = cdo->generic_packet(cdi, &cgc);
	if (ret)
		return ret;

	 
	buflen = be16_to_cpu(di->disc_information_length) +
		sizeof(di->disc_information_length);

	if (buflen > sizeof(disc_information))
		buflen = sizeof(disc_information);

	cgc.cmd[8] = cgc.buflen = buflen;
	ret = cdo->generic_packet(cdi, &cgc);
	if (ret)
		return ret;

	 
	return buflen;
}