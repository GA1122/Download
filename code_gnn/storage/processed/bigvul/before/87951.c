static bool fm10k_tx_desc_push(struct fm10k_ring *tx_ring,
			       struct fm10k_tx_desc *tx_desc, u16 i,
			       dma_addr_t dma, unsigned int size, u8 desc_flags)
{
	 
	if ((++i & (FM10K_TXD_WB_FIFO_SIZE - 1)) == 0)
		desc_flags |= FM10K_TXD_FLAG_RS | FM10K_TXD_FLAG_INT;

	 
	tx_desc->buffer_addr = cpu_to_le64(dma);
	tx_desc->flags = desc_flags;
	tx_desc->buflen = cpu_to_le16(size);

	 
	return i == tx_ring->count;
}