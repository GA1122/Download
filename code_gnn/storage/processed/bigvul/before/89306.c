static int initialize_unit_leds(void)
{
	 
	uchar buf;

	if (i2c_read(BOCO, REG_CTRL_H, 1, &buf, 1) != 0) {
		printf("%s: Error reading Boco\n", __func__);
		return -1;
	}
	buf |= MASK_WRL_UNITRUN;
	if (i2c_write(BOCO, REG_CTRL_H, 1, &buf, 1) != 0) {
		printf("%s: Error writing Boco\n", __func__);
		return -1;
	}
	return 0;
}
