static int tg3_test_dma(struct tg3 *tp)
{
	dma_addr_t buf_dma;
	u32 *buf, saved_dma_rwctrl;
	int ret = 0;

	buf = dma_alloc_coherent(&tp->pdev->dev, TEST_BUFFER_SIZE,
				 &buf_dma, GFP_KERNEL);
	if (!buf) {
		ret = -ENOMEM;
		goto out_nofree;
	}

	tp->dma_rwctrl = ((0x7 << DMA_RWCTRL_PCI_WRITE_CMD_SHIFT) |
			  (0x6 << DMA_RWCTRL_PCI_READ_CMD_SHIFT));

	tp->dma_rwctrl = tg3_calc_dma_bndry(tp, tp->dma_rwctrl);

	if (tg3_flag(tp, 57765_PLUS))
		goto out;

	if (tg3_flag(tp, PCI_EXPRESS)) {
		 
		tp->dma_rwctrl |= 0x00180000;
	} else if (!tg3_flag(tp, PCIX_MODE)) {
		if (tg3_asic_rev(tp) == ASIC_REV_5705 ||
		    tg3_asic_rev(tp) == ASIC_REV_5750)
			tp->dma_rwctrl |= 0x003f0000;
		else
			tp->dma_rwctrl |= 0x003f000f;
	} else {
		if (tg3_asic_rev(tp) == ASIC_REV_5703 ||
		    tg3_asic_rev(tp) == ASIC_REV_5704) {
			u32 ccval = (tr32(TG3PCI_CLOCK_CTRL) & 0x1f);
			u32 read_water = 0x7;

			 
			if (tg3_flag(tp, 40BIT_DMA_BUG) &&
			    tg3_asic_rev(tp) == ASIC_REV_5704)
				tp->dma_rwctrl |= 0x8000;
			else if (ccval == 0x6 || ccval == 0x7)
				tp->dma_rwctrl |= DMA_RWCTRL_ONE_DMA;

			if (tg3_asic_rev(tp) == ASIC_REV_5703)
				read_water = 4;
			 
			tp->dma_rwctrl |=
				(read_water << DMA_RWCTRL_READ_WATER_SHIFT) |
				(0x3 << DMA_RWCTRL_WRITE_WATER_SHIFT) |
				(1 << 23);
		} else if (tg3_asic_rev(tp) == ASIC_REV_5780) {
			 
			tp->dma_rwctrl |= 0x00144000;
		} else if (tg3_asic_rev(tp) == ASIC_REV_5714) {
			 
			tp->dma_rwctrl |= 0x00148000;
		} else {
			tp->dma_rwctrl |= 0x001b000f;
		}
	}
	if (tg3_flag(tp, ONE_DMA_AT_ONCE))
		tp->dma_rwctrl |= DMA_RWCTRL_ONE_DMA;

	if (tg3_asic_rev(tp) == ASIC_REV_5703 ||
	    tg3_asic_rev(tp) == ASIC_REV_5704)
		tp->dma_rwctrl &= 0xfffffff0;

	if (tg3_asic_rev(tp) == ASIC_REV_5700 ||
	    tg3_asic_rev(tp) == ASIC_REV_5701) {
		 
		tp->dma_rwctrl |= DMA_RWCTRL_USE_MEM_READ_MULT;

		 
		tp->dma_rwctrl |= DMA_RWCTRL_ASSERT_ALL_BE;
	}

	tw32(TG3PCI_DMA_RW_CTRL, tp->dma_rwctrl);

#if 0
	 
	tg3_switch_clocks(tp);
#endif

	if (tg3_asic_rev(tp) != ASIC_REV_5700 &&
	    tg3_asic_rev(tp) != ASIC_REV_5701)
		goto out;

	 
	saved_dma_rwctrl = tp->dma_rwctrl;
	tp->dma_rwctrl &= ~DMA_RWCTRL_WRITE_BNDRY_MASK;
	tw32(TG3PCI_DMA_RW_CTRL, tp->dma_rwctrl);

	while (1) {
		u32 *p = buf, i;

		for (i = 0; i < TEST_BUFFER_SIZE / sizeof(u32); i++)
			p[i] = i;

		 
		ret = tg3_do_test_dma(tp, buf, buf_dma, TEST_BUFFER_SIZE, 1);
		if (ret) {
			dev_err(&tp->pdev->dev,
				"%s: Buffer write failed. err = %d\n",
				__func__, ret);
			break;
		}

#if 0
		 
		for (i = 0; i < TEST_BUFFER_SIZE / sizeof(u32); i++) {
			u32 val;
			tg3_read_mem(tp, 0x2100 + (i*4), &val);
			if (le32_to_cpu(val) != p[i]) {
				dev_err(&tp->pdev->dev,
					"%s: Buffer corrupted on device! "
					"(%d != %d)\n", __func__, val, i);
				 
			}
			p[i] = 0;
		}
#endif
		 
		ret = tg3_do_test_dma(tp, buf, buf_dma, TEST_BUFFER_SIZE, 0);
		if (ret) {
			dev_err(&tp->pdev->dev, "%s: Buffer read failed. "
				"err = %d\n", __func__, ret);
			break;
		}

		 
		for (i = 0; i < TEST_BUFFER_SIZE / sizeof(u32); i++) {
			if (p[i] == i)
				continue;

			if ((tp->dma_rwctrl & DMA_RWCTRL_WRITE_BNDRY_MASK) !=
			    DMA_RWCTRL_WRITE_BNDRY_16) {
				tp->dma_rwctrl &= ~DMA_RWCTRL_WRITE_BNDRY_MASK;
				tp->dma_rwctrl |= DMA_RWCTRL_WRITE_BNDRY_16;
				tw32(TG3PCI_DMA_RW_CTRL, tp->dma_rwctrl);
				break;
			} else {
				dev_err(&tp->pdev->dev,
					"%s: Buffer corrupted on read back! "
					"(%d != %d)\n", __func__, p[i], i);
				ret = -ENODEV;
				goto out;
			}
		}

		if (i == (TEST_BUFFER_SIZE / sizeof(u32))) {
			 
			ret = 0;
			break;
		}
	}
	if ((tp->dma_rwctrl & DMA_RWCTRL_WRITE_BNDRY_MASK) !=
	    DMA_RWCTRL_WRITE_BNDRY_16) {
		 
		if (pci_dev_present(tg3_dma_wait_state_chipsets)) {
			tp->dma_rwctrl &= ~DMA_RWCTRL_WRITE_BNDRY_MASK;
			tp->dma_rwctrl |= DMA_RWCTRL_WRITE_BNDRY_16;
		} else {
			 
			tp->dma_rwctrl = saved_dma_rwctrl;
		}

		tw32(TG3PCI_DMA_RW_CTRL, tp->dma_rwctrl);
	}

out:
	dma_free_coherent(&tp->pdev->dev, TEST_BUFFER_SIZE, buf, buf_dma);
out_nofree:
	return ret;
}
