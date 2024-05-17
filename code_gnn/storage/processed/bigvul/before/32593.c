static int tg3_init_one(struct pci_dev *pdev,
				  const struct pci_device_id *ent)
{
	struct net_device *dev;
	struct tg3 *tp;
	int i, err, pm_cap;
	u32 sndmbx, rcvmbx, intmbx;
	char str[40];
	u64 dma_mask, persist_dma_mask;
	netdev_features_t features = 0;

	printk_once(KERN_INFO "%s\n", version);

	err = pci_enable_device(pdev);
	if (err) {
		dev_err(&pdev->dev, "Cannot enable PCI device, aborting\n");
		return err;
	}

	err = pci_request_regions(pdev, DRV_MODULE_NAME);
	if (err) {
		dev_err(&pdev->dev, "Cannot obtain PCI resources, aborting\n");
		goto err_out_disable_pdev;
	}

	pci_set_master(pdev);

	 
	pm_cap = pci_find_capability(pdev, PCI_CAP_ID_PM);
	if (pm_cap == 0) {
		dev_err(&pdev->dev,
			"Cannot find Power Management capability, aborting\n");
		err = -EIO;
		goto err_out_free_res;
	}

	err = pci_set_power_state(pdev, PCI_D0);
	if (err) {
		dev_err(&pdev->dev, "Transition to D0 failed, aborting\n");
		goto err_out_free_res;
	}

	dev = alloc_etherdev_mq(sizeof(*tp), TG3_IRQ_MAX_VECS);
	if (!dev) {
		err = -ENOMEM;
		goto err_out_power_down;
	}

	SET_NETDEV_DEV(dev, &pdev->dev);

	tp = netdev_priv(dev);
	tp->pdev = pdev;
	tp->dev = dev;
	tp->pm_cap = pm_cap;
	tp->rx_mode = TG3_DEF_RX_MODE;
	tp->tx_mode = TG3_DEF_TX_MODE;
	tp->irq_sync = 1;

	if (tg3_debug > 0)
		tp->msg_enable = tg3_debug;
	else
		tp->msg_enable = TG3_DEF_MSG_ENABLE;

	if (pdev_is_ssb_gige_core(pdev)) {
		tg3_flag_set(tp, IS_SSB_CORE);
		if (ssb_gige_must_flush_posted_writes(pdev))
			tg3_flag_set(tp, FLUSH_POSTED_WRITES);
		if (ssb_gige_one_dma_at_once(pdev))
			tg3_flag_set(tp, ONE_DMA_AT_ONCE);
		if (ssb_gige_have_roboswitch(pdev))
			tg3_flag_set(tp, ROBOSWITCH);
		if (ssb_gige_is_rgmii(pdev))
			tg3_flag_set(tp, RGMII_MODE);
	}

	 
	tp->misc_host_ctrl =
		MISC_HOST_CTRL_MASK_PCI_INT |
		MISC_HOST_CTRL_WORD_SWAP |
		MISC_HOST_CTRL_INDIR_ACCESS |
		MISC_HOST_CTRL_PCISTATE_RW;

	 
	tp->grc_mode = (GRC_MODE_WSWAP_DATA | GRC_MODE_BSWAP_DATA |
			GRC_MODE_WSWAP_NONFRM_DATA);
#ifdef __BIG_ENDIAN
	tp->grc_mode |= GRC_MODE_BSWAP_NONFRM_DATA;
#endif
	spin_lock_init(&tp->lock);
	spin_lock_init(&tp->indirect_lock);
	INIT_WORK(&tp->reset_task, tg3_reset_task);

	tp->regs = pci_ioremap_bar(pdev, BAR_0);
	if (!tp->regs) {
		dev_err(&pdev->dev, "Cannot map device registers, aborting\n");
		err = -ENOMEM;
		goto err_out_free_dev;
	}

	if (tp->pdev->device == PCI_DEVICE_ID_TIGON3_5761 ||
	    tp->pdev->device == PCI_DEVICE_ID_TIGON3_5761E ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5761S ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5761SE ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5717 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5717_C ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5718 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5719 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5720 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5762 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5725 ||
	    tp->pdev->device == TG3PCI_DEVICE_TIGON3_5727) {
		tg3_flag_set(tp, ENABLE_APE);
		tp->aperegs = pci_ioremap_bar(pdev, BAR_2);
		if (!tp->aperegs) {
			dev_err(&pdev->dev,
				"Cannot map APE registers, aborting\n");
			err = -ENOMEM;
			goto err_out_iounmap;
		}
	}

	tp->rx_pending = TG3_DEF_RX_RING_PENDING;
	tp->rx_jumbo_pending = TG3_DEF_RX_JUMBO_RING_PENDING;

	dev->ethtool_ops = &tg3_ethtool_ops;
	dev->watchdog_timeo = TG3_TX_TIMEOUT;
	dev->netdev_ops = &tg3_netdev_ops;
	dev->irq = pdev->irq;

	err = tg3_get_invariants(tp, ent);
	if (err) {
		dev_err(&pdev->dev,
			"Problem fetching invariants of chip, aborting\n");
		goto err_out_apeunmap;
	}

	 
	if (tg3_flag(tp, IS_5788))
		persist_dma_mask = dma_mask = DMA_BIT_MASK(32);
	else if (tg3_flag(tp, 40BIT_DMA_BUG)) {
		persist_dma_mask = dma_mask = DMA_BIT_MASK(40);
#ifdef CONFIG_HIGHMEM
		dma_mask = DMA_BIT_MASK(64);
#endif
	} else
		persist_dma_mask = dma_mask = DMA_BIT_MASK(64);

	 
	if (dma_mask > DMA_BIT_MASK(32)) {
		err = pci_set_dma_mask(pdev, dma_mask);
		if (!err) {
			features |= NETIF_F_HIGHDMA;
			err = pci_set_consistent_dma_mask(pdev,
							  persist_dma_mask);
			if (err < 0) {
				dev_err(&pdev->dev, "Unable to obtain 64 bit "
					"DMA for consistent allocations\n");
				goto err_out_apeunmap;
			}
		}
	}
	if (err || dma_mask == DMA_BIT_MASK(32)) {
		err = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
		if (err) {
			dev_err(&pdev->dev,
				"No usable DMA configuration, aborting\n");
			goto err_out_apeunmap;
		}
	}

	tg3_init_bufmgr_config(tp);

	features |= NETIF_F_HW_VLAN_TX | NETIF_F_HW_VLAN_RX;

	 
	if (tg3_chip_rev_id(tp) != CHIPREV_ID_5700_B0) {
		features |= NETIF_F_SG | NETIF_F_IP_CSUM | NETIF_F_RXCSUM;

		if (tg3_flag(tp, 5755_PLUS))
			features |= NETIF_F_IPV6_CSUM;
	}

	 
	if ((tg3_flag(tp, HW_TSO_1) ||
	     tg3_flag(tp, HW_TSO_2) ||
	     tg3_flag(tp, HW_TSO_3)) &&
	    (features & NETIF_F_IP_CSUM))
		features |= NETIF_F_TSO;
	if (tg3_flag(tp, HW_TSO_2) || tg3_flag(tp, HW_TSO_3)) {
		if (features & NETIF_F_IPV6_CSUM)
			features |= NETIF_F_TSO6;
		if (tg3_flag(tp, HW_TSO_3) ||
		    tg3_asic_rev(tp) == ASIC_REV_5761 ||
		    (tg3_asic_rev(tp) == ASIC_REV_5784 &&
		     tg3_chip_rev(tp) != CHIPREV_5784_AX) ||
		    tg3_asic_rev(tp) == ASIC_REV_5785 ||
		    tg3_asic_rev(tp) == ASIC_REV_57780)
			features |= NETIF_F_TSO_ECN;
	}

	dev->features |= features;
	dev->vlan_features |= features;

	 
	if (tg3_asic_rev(tp) != ASIC_REV_5780 &&
	    !tg3_flag(tp, CPMU_PRESENT))
		 
		features |= NETIF_F_LOOPBACK;

	dev->hw_features |= features;

	if (tg3_chip_rev_id(tp) == CHIPREV_ID_5705_A1 &&
	    !tg3_flag(tp, TSO_CAPABLE) &&
	    !(tr32(TG3PCI_PCISTATE) & PCISTATE_BUS_SPEED_HIGH)) {
		tg3_flag_set(tp, MAX_RXPEND_64);
		tp->rx_pending = 63;
	}

	err = tg3_get_device_address(tp);
	if (err) {
		dev_err(&pdev->dev,
			"Could not obtain valid ethernet address, aborting\n");
		goto err_out_apeunmap;
	}

	 
	if ((tr32(HOSTCC_MODE) & HOSTCC_MODE_ENABLE) ||
	    (tr32(WDMAC_MODE) & WDMAC_MODE_ENABLE)) {
		tw32(MEMARB_MODE, MEMARB_MODE_ENABLE);
		tg3_halt(tp, RESET_KIND_SHUTDOWN, 1);
	}

	err = tg3_test_dma(tp);
	if (err) {
		dev_err(&pdev->dev, "DMA engine test failed, aborting\n");
		goto err_out_apeunmap;
	}

	intmbx = MAILBOX_INTERRUPT_0 + TG3_64BIT_REG_LOW;
	rcvmbx = MAILBOX_RCVRET_CON_IDX_0 + TG3_64BIT_REG_LOW;
	sndmbx = MAILBOX_SNDHOST_PROD_IDX_0 + TG3_64BIT_REG_LOW;
	for (i = 0; i < tp->irq_max; i++) {
		struct tg3_napi *tnapi = &tp->napi[i];

		tnapi->tp = tp;
		tnapi->tx_pending = TG3_DEF_TX_RING_PENDING;

		tnapi->int_mbox = intmbx;
		if (i <= 4)
			intmbx += 0x8;
		else
			intmbx += 0x4;

		tnapi->consmbox = rcvmbx;
		tnapi->prodmbox = sndmbx;

		if (i)
			tnapi->coal_now = HOSTCC_MODE_COAL_VEC1_NOW << (i - 1);
		else
			tnapi->coal_now = HOSTCC_MODE_NOW;

		if (!tg3_flag(tp, SUPPORT_MSIX))
			break;

		 
		if (!i)
			continue;

		rcvmbx += 0x8;

		if (sndmbx & 0x4)
			sndmbx -= 0x4;
		else
			sndmbx += 0xc;
	}

	tg3_init_coal(tp);

	pci_set_drvdata(pdev, dev);

	if (tg3_asic_rev(tp) == ASIC_REV_5719 ||
	    tg3_asic_rev(tp) == ASIC_REV_5720 ||
	    tg3_asic_rev(tp) == ASIC_REV_5762)
		tg3_flag_set(tp, PTP_CAPABLE);

	if (tg3_flag(tp, 5717_PLUS)) {
		 
		tg3_frob_aux_power(tp, false);
	}

	tg3_timer_init(tp);

	tg3_carrier_off(tp);

	err = register_netdev(dev);
	if (err) {
		dev_err(&pdev->dev, "Cannot register net device, aborting\n");
		goto err_out_apeunmap;
	}

	netdev_info(dev, "Tigon3 [partno(%s) rev %04x] (%s) MAC address %pM\n",
		    tp->board_part_number,
		    tg3_chip_rev_id(tp),
		    tg3_bus_string(tp, str),
		    dev->dev_addr);

	if (tp->phy_flags & TG3_PHYFLG_IS_CONNECTED) {
		struct phy_device *phydev;
		phydev = tp->mdio_bus->phy_map[TG3_PHY_MII_ADDR];
		netdev_info(dev,
			    "attached PHY driver [%s] (mii_bus:phy_addr=%s)\n",
			    phydev->drv->name, dev_name(&phydev->dev));
	} else {
		char *ethtype;

		if (tp->phy_flags & TG3_PHYFLG_10_100_ONLY)
			ethtype = "10/100Base-TX";
		else if (tp->phy_flags & TG3_PHYFLG_ANY_SERDES)
			ethtype = "1000Base-SX";
		else
			ethtype = "10/100/1000Base-T";

		netdev_info(dev, "attached PHY is %s (%s Ethernet) "
			    "(WireSpeed[%d], EEE[%d])\n",
			    tg3_phy_string(tp), ethtype,
			    (tp->phy_flags & TG3_PHYFLG_NO_ETH_WIRE_SPEED) == 0,
			    (tp->phy_flags & TG3_PHYFLG_EEE_CAP) != 0);
	}

	netdev_info(dev, "RXcsums[%d] LinkChgREG[%d] MIirq[%d] ASF[%d] TSOcap[%d]\n",
		    (dev->features & NETIF_F_RXCSUM) != 0,
		    tg3_flag(tp, USE_LINKCHG_REG) != 0,
		    (tp->phy_flags & TG3_PHYFLG_USE_MI_INTERRUPT) != 0,
		    tg3_flag(tp, ENABLE_ASF) != 0,
		    tg3_flag(tp, TSO_CAPABLE) != 0);
	netdev_info(dev, "dma_rwctrl[%08x] dma_mask[%d-bit]\n",
		    tp->dma_rwctrl,
		    pdev->dma_mask == DMA_BIT_MASK(32) ? 32 :
		    ((u64)pdev->dma_mask) == DMA_BIT_MASK(40) ? 40 : 64);

	pci_save_state(pdev);

	return 0;

err_out_apeunmap:
	if (tp->aperegs) {
		iounmap(tp->aperegs);
		tp->aperegs = NULL;
	}

err_out_iounmap:
	if (tp->regs) {
		iounmap(tp->regs);
		tp->regs = NULL;
	}

err_out_free_dev:
	free_netdev(dev);

err_out_power_down:
	pci_set_power_state(pdev, PCI_D3hot);

err_out_free_res:
	pci_release_regions(pdev);

err_out_disable_pdev:
	pci_disable_device(pdev);
	pci_set_drvdata(pdev, NULL);
	return err;
}
