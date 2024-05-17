static void cqspi_controller_init(struct cqspi_st *cqspi)
{
	cqspi_controller_enable(cqspi, 0);

	 
	writel(0, cqspi->iobase + CQSPI_REG_REMAP);

	 
	writel(0, cqspi->iobase + CQSPI_REG_IRQMASK);

	 
	writel(cqspi->fifo_depth / 2, cqspi->iobase + CQSPI_REG_SRAMPARTITION);

	 
	writel(cqspi->trigger_address,
	       cqspi->iobase + CQSPI_REG_INDIRECTTRIGGER);

	 
	writel(cqspi->fifo_depth * cqspi->fifo_width / 2,
	       cqspi->iobase + CQSPI_REG_INDIRECTRDWATERMARK);
	 
	writel(cqspi->fifo_depth * cqspi->fifo_width / 8,
	       cqspi->iobase + CQSPI_REG_INDIRECTWRWATERMARK);

	cqspi_controller_enable(cqspi, 1);
}
