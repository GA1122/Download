void pci_init_board(void)
{
	qrio_prstcfg(PCIE_SW_RST, PRSTCFG_POWUP_UNIT_CORE_RST);
	qrio_prstcfg(PEXHC_RST, PRSTCFG_POWUP_UNIT_CORE_RST);
	qrio_prstcfg(HOOPER_RST, PRSTCFG_POWUP_UNIT_CORE_RST);

	 
	if (wait_for_fpga_config())
		printf("error finishing PCIe FPGA config\n");

	qrio_prst(PCIE_SW_RST, false, false);
	qrio_prst(PEXHC_RST, false, false);
	qrio_prst(HOOPER_RST, false, false);
	 
	mdelay(50);

	fsl_pcie_init_board(0);
}
