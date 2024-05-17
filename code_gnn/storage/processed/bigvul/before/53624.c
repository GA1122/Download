static struct usb_cdc_ncm_ndp16 *cdc_ncm_ndp(struct cdc_ncm_ctx *ctx, struct sk_buff *skb, __le32 sign, size_t reserve)
{
	struct usb_cdc_ncm_ndp16 *ndp16 = NULL;
	struct usb_cdc_ncm_nth16 *nth16 = (void *)skb->data;
	size_t ndpoffset = le16_to_cpu(nth16->wNdpIndex);

	 
	if (ctx->drvflags & CDC_NCM_FLAG_NDP_TO_END) {
		if (ctx->delayed_ndp16->dwSignature == sign)
			return ctx->delayed_ndp16;

		 
		else if (ctx->delayed_ndp16->dwSignature)
			return NULL;
	}

	 
	while (ndpoffset) {
		ndp16 = (struct usb_cdc_ncm_ndp16 *)(skb->data + ndpoffset);
		if  (ndp16->dwSignature == sign)
			return ndp16;
		ndpoffset = le16_to_cpu(ndp16->wNextNdpIndex);
	}

	 
	if (!(ctx->drvflags & CDC_NCM_FLAG_NDP_TO_END))
		cdc_ncm_align_tail(skb, ctx->tx_ndp_modulus, 0, ctx->tx_max);

	 
	if ((ctx->tx_max - skb->len - reserve) < ctx->max_ndp_size)
		return NULL;

	 
	if (ndp16)
		ndp16->wNextNdpIndex = cpu_to_le16(skb->len);
	else
		nth16->wNdpIndex = cpu_to_le16(skb->len);

	 
	if (!(ctx->drvflags & CDC_NCM_FLAG_NDP_TO_END))
		ndp16 = (struct usb_cdc_ncm_ndp16 *)memset(skb_put(skb, ctx->max_ndp_size), 0, ctx->max_ndp_size);
	else
		ndp16 = ctx->delayed_ndp16;

	ndp16->dwSignature = sign;
	ndp16->wLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_ndp16) + sizeof(struct usb_cdc_ncm_dpe16));
	return ndp16;
}
