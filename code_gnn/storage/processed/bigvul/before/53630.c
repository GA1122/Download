static int cdc_ncm_setup(struct usbnet *dev)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	u32 def_rx, def_tx;

	 
	def_rx = min_t(u32, CDC_NCM_NTB_DEF_SIZE_RX,
		       le32_to_cpu(ctx->ncm_parm.dwNtbInMaxSize));
	def_tx = min_t(u32, CDC_NCM_NTB_DEF_SIZE_TX,
		       le32_to_cpu(ctx->ncm_parm.dwNtbOutMaxSize));

	 
	cdc_ncm_update_rxtx_max(dev, def_rx, def_tx);

	 
	cdc_ncm_fix_modulus(dev);

	 
	cdc_ncm_set_dgram_size(dev, cdc_ncm_max_dgram_size(dev));
	return 0;
}
