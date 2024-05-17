efx_max_tx_len(struct efx_nic *efx, dma_addr_t dma_addr)
{
	 
	unsigned len = (~dma_addr & 0xfff) + 1;

	 
	if (EFX_WORKAROUND_5391(efx) && (dma_addr & 0xf))
		len = min_t(unsigned, len, 512 - (dma_addr & 0xf));

 	return len;
 }
