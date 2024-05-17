int wait_for_fpga_config(void)
{
	int ret = 0;
	u8 spictrl;
	u32 timeout = 20000;

	if (skip)
		return 0;

	if (!check_boco2()) {
		 
		return 0;
	}

	printf("PCIe FPGA config:");
	do {
		ret = i2c_read(BOCO_ADDR, SPI_REG, 1, &spictrl, 1);
		if (ret) {
			printf("%s: error reading the BOCO spictrl !!\n",
				__func__);
			return ret;
		}
		if (timeout-- == 0) {
			printf(" FPGA_DONE timeout\n");
			return -EFAULT;
		}
		udelay(10);
	} while (!(spictrl & FPGA_DONE));

	printf(" done\n");

 	return 0;
 }
