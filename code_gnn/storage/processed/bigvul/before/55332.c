static s32 atl2_reset_hw(struct atl2_hw *hw)
{
	u32 icr;
	u16 pci_cfg_cmd_word;
	int i;

	 
	atl2_read_pci_cfg(hw, PCI_REG_COMMAND, &pci_cfg_cmd_word);
	if ((pci_cfg_cmd_word &
		(CMD_IO_SPACE|CMD_MEMORY_SPACE|CMD_BUS_MASTER)) !=
		(CMD_IO_SPACE|CMD_MEMORY_SPACE|CMD_BUS_MASTER)) {
		pci_cfg_cmd_word |=
			(CMD_IO_SPACE|CMD_MEMORY_SPACE|CMD_BUS_MASTER);
		atl2_write_pci_cfg(hw, PCI_REG_COMMAND, &pci_cfg_cmd_word);
	}

	 
	 
	 
	 

	 
	ATL2_WRITE_REG(hw, REG_MASTER_CTRL, MASTER_CTRL_SOFT_RST);
	wmb();
	msleep(1);  

	 
	for (i = 0; i < 10; i++) {
		icr = ATL2_READ_REG(hw, REG_IDLE_STATUS);
		if (!icr)
			break;
		msleep(1);  
		cpu_relax();
	}

	if (icr)
		return icr;

	return 0;
}
