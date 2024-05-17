static int check_boco2(void)
{
	int ret;
	u8 id;

	ret = i2c_read(BOCO_ADDR, ID_REG, 1, &id, 1);
	if (ret) {
		printf("%s: error reading the BOCO id !!\n", __func__);
		return ret;
	}

	return (id == BOCO2_ID);
}
