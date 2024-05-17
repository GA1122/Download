int ethernet_present(void)
{
	uchar	buf;
	int	ret = 0;

	if (i2c_read(BOCO, REG_CTRL_H, 1, &buf, 1) != 0) {
		printf("%s: Error reading Boco\n", __func__);
		return -1;
	}
	if ((buf & MASK_RBX_PGY_PRESENT) == MASK_RBX_PGY_PRESENT)
		ret = 1;

	return ret;
}
