 static int fpga_done(void)
 {
 	int ret = 0;
	u8 regval;

	 
	if (!check_boco2())
		return 0;

	ret = i2c_read(BOCO_ADDR, SPI_REG, 1, &regval, 1);
	if (ret) {
		printf("%s: error reading the BOCO @%#x !!\n",
			__func__, SPI_REG);
		return 0;
	}
 
 	return regval & FPGA_DONE ? 1 : 0;
 }
