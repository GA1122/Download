static int fpga_download(int iobase, int bitrate)
{
	int i, rc;
	unsigned char *pbits;

	pbits = get_mcs(bitrate);
	if (pbits == NULL)
		return -1;

	fpga_reset(iobase);
	for (i = 0; i < YAM_FPGA_SIZE; i++) {
		if (fpga_write(iobase, pbits[i])) {
			printk(KERN_ERR "yam: error in write cycle\n");
			return -1;			 
		}
	}

	fpga_write(iobase, 0xFF);
	rc = inb(MSR(iobase));		 

	 
	delay(50);

	return (rc & MSR_DSR) ? 0 : -1;
}
