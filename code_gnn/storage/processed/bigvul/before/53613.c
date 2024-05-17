cdc_ncm_fill_tx_frame(struct usbnet *dev, struct sk_buff *skb, __le32 sign)
{
	struct cdc_ncm_ctx *ctx = (struct cdc_ncm_ctx *)dev->data[0];
	struct usb_cdc_ncm_nth16 *nth16;
	struct usb_cdc_ncm_ndp16 *ndp16;
	struct sk_buff *skb_out;
	u16 n = 0, index, ndplen;
	u8 ready2send = 0;
	u32 delayed_ndp_size;

	 
	if (ctx->drvflags & CDC_NCM_FLAG_NDP_TO_END)
		delayed_ndp_size = ctx->max_ndp_size;
	else
		delayed_ndp_size = 0;

	 
	if (skb != NULL) {
		swap(skb, ctx->tx_rem_skb);
		swap(sign, ctx->tx_rem_sign);
	} else {
		ready2send = 1;
	}

	 
	skb_out = ctx->tx_curr_skb;

	 
	if (!skb_out) {
		skb_out = alloc_skb(ctx->tx_max, GFP_ATOMIC);
		if (skb_out == NULL) {
			if (skb != NULL) {
				dev_kfree_skb_any(skb);
				dev->net->stats.tx_dropped++;
			}
			goto exit_no_skb;
		}
		 
		nth16 = (struct usb_cdc_ncm_nth16 *)memset(skb_put(skb_out, sizeof(struct usb_cdc_ncm_nth16)), 0, sizeof(struct usb_cdc_ncm_nth16));
		nth16->dwSignature = cpu_to_le32(USB_CDC_NCM_NTH16_SIGN);
		nth16->wHeaderLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_nth16));
		nth16->wSequence = cpu_to_le16(ctx->tx_seq++);

		 
		ctx->tx_curr_frame_num = 0;

		 
		ctx->tx_curr_frame_payload = 0;
	}

	for (n = ctx->tx_curr_frame_num; n < ctx->tx_max_datagrams; n++) {
		 
		if (skb == NULL) {
			skb = ctx->tx_rem_skb;
			sign = ctx->tx_rem_sign;
			ctx->tx_rem_skb = NULL;

			 
			if (skb == NULL)
				break;
		}

		 
		ndp16 = cdc_ncm_ndp(ctx, skb_out, sign, skb->len + ctx->tx_modulus + ctx->tx_remainder);

		 
		cdc_ncm_align_tail(skb_out,  ctx->tx_modulus, ctx->tx_remainder, ctx->tx_max);

		 
		if (!ndp16 || skb_out->len + skb->len + delayed_ndp_size > ctx->tx_max) {
			if (n == 0) {
				 
				dev_kfree_skb_any(skb);
				skb = NULL;
				dev->net->stats.tx_dropped++;
			} else {
				 
				if (ctx->tx_rem_skb != NULL) {
					dev_kfree_skb_any(ctx->tx_rem_skb);
					dev->net->stats.tx_dropped++;
				}
				ctx->tx_rem_skb = skb;
				ctx->tx_rem_sign = sign;
				skb = NULL;
				ready2send = 1;
				ctx->tx_reason_ntb_full++;	 
			}
			break;
		}

		 
		ndplen = le16_to_cpu(ndp16->wLength);
		index = (ndplen - sizeof(struct usb_cdc_ncm_ndp16)) / sizeof(struct usb_cdc_ncm_dpe16) - 1;

		 
		ndp16->dpe16[index].wDatagramLength = cpu_to_le16(skb->len);
		ndp16->dpe16[index].wDatagramIndex = cpu_to_le16(skb_out->len);
		ndp16->wLength = cpu_to_le16(ndplen + sizeof(struct usb_cdc_ncm_dpe16));
		memcpy(skb_put(skb_out, skb->len), skb->data, skb->len);
		ctx->tx_curr_frame_payload += skb->len;	 
		dev_kfree_skb_any(skb);
		skb = NULL;

		 
		if (index >= CDC_NCM_DPT_DATAGRAMS_MAX) {
			ready2send = 1;
			ctx->tx_reason_ndp_full++;	 
			break;
		}
	}

	 
	if (skb != NULL) {
		dev_kfree_skb_any(skb);
		skb = NULL;
		dev->net->stats.tx_dropped++;
	}

	ctx->tx_curr_frame_num = n;

	if (n == 0) {
		 
		 
		ctx->tx_curr_skb = skb_out;
		goto exit_no_skb;

	} else if ((n < ctx->tx_max_datagrams) && (ready2send == 0) && (ctx->timer_interval > 0)) {
		 
		 
		ctx->tx_curr_skb = skb_out;
		 
		if (n < CDC_NCM_RESTART_TIMER_DATAGRAM_CNT)
			ctx->tx_timer_pending = CDC_NCM_TIMER_PENDING_CNT;
		goto exit_no_skb;

	} else {
		if (n == ctx->tx_max_datagrams)
			ctx->tx_reason_max_datagram++;	 
		 
		 
	}

	 
	if (ctx->drvflags & CDC_NCM_FLAG_NDP_TO_END) {
		nth16 = (struct usb_cdc_ncm_nth16 *)skb_out->data;
		cdc_ncm_align_tail(skb_out, ctx->tx_ndp_modulus, 0, ctx->tx_max);
		nth16->wNdpIndex = cpu_to_le16(skb_out->len);
		memcpy(skb_put(skb_out, ctx->max_ndp_size), ctx->delayed_ndp16, ctx->max_ndp_size);

		 
		ndp16 = memset(ctx->delayed_ndp16, 0, ctx->max_ndp_size);
	}

	 
	if (!(dev->driver_info->flags & FLAG_SEND_ZLP) &&
	    skb_out->len > ctx->min_tx_pkt)
		memset(skb_put(skb_out, ctx->tx_max - skb_out->len), 0,
		       ctx->tx_max - skb_out->len);
	else if (skb_out->len < ctx->tx_max && (skb_out->len % dev->maxpacket) == 0)
		*skb_put(skb_out, 1) = 0;	 

	 
	nth16 = (struct usb_cdc_ncm_nth16 *)skb_out->data;
	nth16->wBlockLength = cpu_to_le16(skb_out->len);

	 
	ctx->tx_curr_skb = NULL;

	 
	ctx->tx_overhead += skb_out->len - ctx->tx_curr_frame_payload;
	ctx->tx_ntbs++;

	 
	usbnet_set_skb_tx_stats(skb_out, n,
				(long)ctx->tx_curr_frame_payload - skb_out->len);

	return skb_out;

exit_no_skb:
	 
	if (ctx->tx_curr_skb != NULL && n > 0)
		cdc_ncm_tx_timeout_start(ctx);
	return NULL;
}
