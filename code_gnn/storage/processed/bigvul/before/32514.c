static int tg3_chip_reset(struct tg3 *tp)
{
	u32 val;
	void (*write_op)(struct tg3 *, u32, u32);
	int i, err;

	tg3_nvram_lock(tp);

	tg3_ape_lock(tp, TG3_APE_LOCK_GRC);

	 
	tp->nvram_lock_cnt = 0;

	 
	tg3_save_pci_state(tp);

	if (tg3_asic_rev(tp) == ASIC_REV_5752 ||
	    tg3_flag(tp, 5755_PLUS))
		tw32(GRC_FASTBOOT_PC, 0);

	 
	write_op = tp->write32;
	if (write_op == tg3_write_flush_reg32)
		tp->write32 = tg3_write32;

	 
	tg3_flag_set(tp, CHIP_RESETTING);
	for (i = 0; i < tp->irq_cnt; i++) {
		struct tg3_napi *tnapi = &tp->napi[i];
		if (tnapi->hw_status) {
			tnapi->hw_status->status = 0;
			tnapi->hw_status->status_tag = 0;
		}
		tnapi->last_tag = 0;
		tnapi->last_irq_tag = 0;
	}
	smp_mb();

	for (i = 0; i < tp->irq_cnt; i++)
		synchronize_irq(tp->napi[i].irq_vec);

	if (tg3_asic_rev(tp) == ASIC_REV_57780) {
		val = tr32(TG3_PCIE_LNKCTL) & ~TG3_PCIE_LNKCTL_L1_PLL_PD_EN;
		tw32(TG3_PCIE_LNKCTL, val | TG3_PCIE_LNKCTL_L1_PLL_PD_DIS);
	}

	 
	val = GRC_MISC_CFG_CORECLK_RESET;

	if (tg3_flag(tp, PCI_EXPRESS)) {
		 
		if (tg3_asic_rev(tp) != ASIC_REV_5785 &&
		    !tg3_flag(tp, 57765_PLUS) &&
		    tr32(TG3_PCIE_PHY_TSTCTL) ==
		    (TG3_PCIE_PHY_TSTCTL_PCIE10 | TG3_PCIE_PHY_TSTCTL_PSCRAM))
			tw32(TG3_PCIE_PHY_TSTCTL, TG3_PCIE_PHY_TSTCTL_PSCRAM);

		if (tg3_chip_rev_id(tp) != CHIPREV_ID_5750_A0) {
			tw32(GRC_MISC_CFG, (1 << 29));
			val |= (1 << 29);
		}
	}

	if (tg3_asic_rev(tp) == ASIC_REV_5906) {
		tw32(VCPU_STATUS, tr32(VCPU_STATUS) | VCPU_STATUS_DRV_RESET);
		tw32(GRC_VCPU_EXT_CTRL,
		     tr32(GRC_VCPU_EXT_CTRL) & ~GRC_VCPU_EXT_CTRL_HALT_CPU);
	}

	 
	if (tg3_flag(tp, 5705_PLUS) && !tg3_flag(tp, CPMU_PRESENT))
		val |= GRC_MISC_CFG_KEEP_GPHY_POWER;

	tw32(GRC_MISC_CFG, val);

	 
	tp->write32 = write_op;

	 
	udelay(120);

	 
	pci_read_config_dword(tp->pdev, PCI_COMMAND, &val);

	udelay(120);

	if (tg3_flag(tp, PCI_EXPRESS) && pci_is_pcie(tp->pdev)) {
		u16 val16;

		if (tg3_chip_rev_id(tp) == CHIPREV_ID_5750_A0) {
			int j;
			u32 cfg_val;

			 
			for (j = 0; j < 5000; j++)
				udelay(100);

			pci_read_config_dword(tp->pdev, 0xc4, &cfg_val);
			pci_write_config_dword(tp->pdev, 0xc4,
					       cfg_val | (1 << 15));
		}

		 
		val16 = PCI_EXP_DEVCTL_RELAX_EN | PCI_EXP_DEVCTL_NOSNOOP_EN;
		 
		if (!tg3_flag(tp, CPMU_PRESENT))
			val16 |= PCI_EXP_DEVCTL_PAYLOAD;
		pcie_capability_clear_word(tp->pdev, PCI_EXP_DEVCTL, val16);

		 
		pcie_capability_write_word(tp->pdev, PCI_EXP_DEVSTA,
				      PCI_EXP_DEVSTA_CED |
				      PCI_EXP_DEVSTA_NFED |
				      PCI_EXP_DEVSTA_FED |
				      PCI_EXP_DEVSTA_URD);
	}

	tg3_restore_pci_state(tp);

	tg3_flag_clear(tp, CHIP_RESETTING);
	tg3_flag_clear(tp, ERROR_PROCESSED);

	val = 0;
	if (tg3_flag(tp, 5780_CLASS))
		val = tr32(MEMARB_MODE);
	tw32(MEMARB_MODE, val | MEMARB_MODE_ENABLE);

	if (tg3_chip_rev_id(tp) == CHIPREV_ID_5750_A3) {
		tg3_stop_fw(tp);
		tw32(0x5000, 0x400);
	}

	if (tg3_flag(tp, IS_SSB_CORE)) {
		 
		tg3_stop_fw(tp);
		tg3_halt_cpu(tp, RX_CPU_BASE);
	}

	tw32(GRC_MODE, tp->grc_mode);

	if (tg3_chip_rev_id(tp) == CHIPREV_ID_5705_A0) {
		val = tr32(0xc4);

		tw32(0xc4, val | (1 << 15));
	}

	if ((tp->nic_sram_data_cfg & NIC_SRAM_DATA_CFG_MINI_PCI) != 0 &&
	    tg3_asic_rev(tp) == ASIC_REV_5705) {
		tp->pci_clock_ctrl |= CLOCK_CTRL_CLKRUN_OENABLE;
		if (tg3_chip_rev_id(tp) == CHIPREV_ID_5705_A0)
			tp->pci_clock_ctrl |= CLOCK_CTRL_FORCE_CLKRUN;
		tw32(TG3PCI_CLOCK_CTRL, tp->pci_clock_ctrl);
	}

	if (tp->phy_flags & TG3_PHYFLG_PHY_SERDES) {
		tp->mac_mode = MAC_MODE_PORT_MODE_TBI;
		val = tp->mac_mode;
	} else if (tp->phy_flags & TG3_PHYFLG_MII_SERDES) {
		tp->mac_mode = MAC_MODE_PORT_MODE_GMII;
		val = tp->mac_mode;
	} else
		val = 0;

	tw32_f(MAC_MODE, val);
	udelay(40);

	tg3_ape_unlock(tp, TG3_APE_LOCK_GRC);

	err = tg3_poll_fw(tp);
	if (err)
		return err;

	tg3_mdio_start(tp);

	if (tg3_flag(tp, PCI_EXPRESS) &&
	    tg3_chip_rev_id(tp) != CHIPREV_ID_5750_A0 &&
	    tg3_asic_rev(tp) != ASIC_REV_5785 &&
	    !tg3_flag(tp, 57765_PLUS)) {
		val = tr32(0x7c00);

		tw32(0x7c00, val | (1 << 25));
	}

	if (tg3_asic_rev(tp) == ASIC_REV_5720) {
		val = tr32(TG3_CPMU_CLCK_ORIDE);
		tw32(TG3_CPMU_CLCK_ORIDE, val & ~CPMU_CLCK_ORIDE_MAC_ORIDE_EN);
	}

	 
	tg3_flag_clear(tp, ENABLE_ASF);
	tg3_flag_clear(tp, ASF_NEW_HANDSHAKE);
	tg3_read_mem(tp, NIC_SRAM_DATA_SIG, &val);
	if (val == NIC_SRAM_DATA_SIG_MAGIC) {
		u32 nic_cfg;

		tg3_read_mem(tp, NIC_SRAM_DATA_CFG, &nic_cfg);
		if (nic_cfg & NIC_SRAM_DATA_CFG_ASF_ENABLE) {
			tg3_flag_set(tp, ENABLE_ASF);
			tp->last_event_jiffies = jiffies;
			if (tg3_flag(tp, 5750_PLUS))
				tg3_flag_set(tp, ASF_NEW_HANDSHAKE);
		}
	}

	return 0;
}