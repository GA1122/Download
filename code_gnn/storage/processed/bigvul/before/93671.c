static int cqspi_indirect_write_execute(struct spi_nor *nor,
					const u8 *txbuf, const unsigned n_tx)
{
	const unsigned int page_size = nor->page_size;
	struct cqspi_flash_pdata *f_pdata = nor->priv;
	struct cqspi_st *cqspi = f_pdata->cqspi;
	void __iomem *reg_base = cqspi->iobase;
	unsigned int remaining = n_tx;
	unsigned int write_bytes;
	int ret;

	writel(remaining, reg_base + CQSPI_REG_INDIRECTWRBYTES);

	 
	writel(CQSPI_IRQ_STATUS_MASK, reg_base + CQSPI_REG_IRQSTATUS);

	writel(CQSPI_IRQ_MASK_WR, reg_base + CQSPI_REG_IRQMASK);

	reinit_completion(&cqspi->transfer_complete);
	writel(CQSPI_REG_INDIRECTWR_START_MASK,
	       reg_base + CQSPI_REG_INDIRECTWR);

	while (remaining > 0) {
		write_bytes = remaining > page_size ? page_size : remaining;
		writesl(cqspi->ahb_base, txbuf, DIV_ROUND_UP(write_bytes, 4));

		ret = wait_for_completion_timeout(&cqspi->transfer_complete,
						  msecs_to_jiffies
						  (CQSPI_TIMEOUT_MS));
		if (!ret) {
			dev_err(nor->dev, "Indirect write timeout\n");
			ret = -ETIMEDOUT;
			goto failwr;
		}

		txbuf += write_bytes;
		remaining -= write_bytes;

		if (remaining > 0)
			reinit_completion(&cqspi->transfer_complete);
	}

	 
	ret = cqspi_wait_for_bit(reg_base + CQSPI_REG_INDIRECTWR,
				 CQSPI_REG_INDIRECTWR_DONE_MASK, 0);
	if (ret) {
		dev_err(nor->dev,
			"Indirect write completion error (%i)\n", ret);
		goto failwr;
	}

	 
	writel(0, reg_base + CQSPI_REG_IRQMASK);

	 
	writel(CQSPI_REG_INDIRECTWR_DONE_MASK, reg_base + CQSPI_REG_INDIRECTWR);

	cqspi_wait_idle(cqspi);

	return 0;

failwr:
	 
	writel(0, reg_base + CQSPI_REG_IRQMASK);

	 
	writel(CQSPI_REG_INDIRECTWR_CANCEL_MASK,
	       reg_base + CQSPI_REG_INDIRECTWR);
	return ret;
}
