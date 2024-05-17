int fpga_reset(void)
{
	int ret = 0;
	u8 resets;

	if (!check_boco2()) {
		 
		return 0;
	}

	 
	resets = skip ? PCIE_RST : PCIE_RST | TRAFFIC_RST;

	ret = boco_clear_bits(PRST1, resets);
	if (ret)
		return ret;

	 
	udelay(10);

	ret = boco_set_bits(PRST1, resets);
	if (ret)
		return ret;

	return 0;
}
