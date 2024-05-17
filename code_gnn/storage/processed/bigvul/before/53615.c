static void cdc_ncm_fix_modulus(struct usbnet *dev)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	u32 val;

	 
	val = ctx->tx_ndp_modulus;

	if ((val < USB_CDC_NCM_NDP_ALIGN_MIN_SIZE) ||
	    (val != ((-val) & val)) || (val >= ctx->tx_max)) {
		dev_dbg(&dev->intf->dev, "Using default alignment: 4 bytes\n");
		ctx->tx_ndp_modulus = USB_CDC_NCM_NDP_ALIGN_MIN_SIZE;
	}

	 
	val = ctx->tx_modulus;

	if ((val < USB_CDC_NCM_NDP_ALIGN_MIN_SIZE) ||
	    (val != ((-val) & val)) || (val >= ctx->tx_max)) {
		dev_dbg(&dev->intf->dev, "Using default transmit modulus: 4 bytes\n");
		ctx->tx_modulus = USB_CDC_NCM_NDP_ALIGN_MIN_SIZE;
	}

	 
	if (ctx->tx_remainder >= ctx->tx_modulus) {
		dev_dbg(&dev->intf->dev, "Using default transmit remainder: 0 bytes\n");
		ctx->tx_remainder = 0;
	}

	 
	ctx->tx_remainder = ((ctx->tx_remainder - cdc_ncm_eth_hlen(dev)) &
			     (ctx->tx_modulus - 1));
}