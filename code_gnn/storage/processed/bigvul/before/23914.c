static void fr_lmi_send(struct net_device *dev, int fullrep)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	struct sk_buff *skb;
	pvc_device *pvc = state(hdlc)->first_pvc;
	int lmi = state(hdlc)->settings.lmi;
	int dce = state(hdlc)->settings.dce;
	int len = lmi == LMI_ANSI ? LMI_ANSI_LENGTH : LMI_CCITT_CISCO_LENGTH;
	int stat_len = (lmi == LMI_CISCO) ? 6 : 3;
	u8 *data;
	int i = 0;

	if (dce && fullrep) {
		len += state(hdlc)->dce_pvc_count * (2 + stat_len);
		if (len > HDLC_MAX_MRU) {
			netdev_warn(dev, "Too many PVCs while sending LMI full report\n");
			return;
		}
	}

	skb = dev_alloc_skb(len);
	if (!skb) {
		netdev_warn(dev, "Memory squeeze on fr_lmi_send()\n");
		return;
	}
	memset(skb->data, 0, len);
	skb_reserve(skb, 4);
	if (lmi == LMI_CISCO) {
		skb->protocol = cpu_to_be16(NLPID_CISCO_LMI);
		fr_hard_header(&skb, LMI_CISCO_DLCI);
	} else {
		skb->protocol = cpu_to_be16(NLPID_CCITT_ANSI_LMI);
		fr_hard_header(&skb, LMI_CCITT_ANSI_DLCI);
	}
	data = skb_tail_pointer(skb);
	data[i++] = LMI_CALLREF;
	data[i++] = dce ? LMI_STATUS : LMI_STATUS_ENQUIRY;
	if (lmi == LMI_ANSI)
		data[i++] = LMI_ANSI_LOCKSHIFT;
	data[i++] = lmi == LMI_CCITT ? LMI_CCITT_REPTYPE :
		LMI_ANSI_CISCO_REPTYPE;
	data[i++] = LMI_REPT_LEN;
	data[i++] = fullrep ? LMI_FULLREP : LMI_INTEGRITY;
	data[i++] = lmi == LMI_CCITT ? LMI_CCITT_ALIVE : LMI_ANSI_CISCO_ALIVE;
	data[i++] = LMI_INTEG_LEN;
	data[i++] = state(hdlc)->txseq =
		fr_lmi_nextseq(state(hdlc)->txseq);
	data[i++] = state(hdlc)->rxseq;

	if (dce && fullrep) {
		while (pvc) {
			data[i++] = lmi == LMI_CCITT ? LMI_CCITT_PVCSTAT :
				LMI_ANSI_CISCO_PVCSTAT;
			data[i++] = stat_len;

			 
			if (state(hdlc)->reliable && !pvc->state.exist) {
				pvc->state.exist = pvc->state.new = 1;
				fr_log_dlci_active(pvc);
			}

			 
			if (pvc->open_count && !pvc->state.active &&
			    pvc->state.exist && !pvc->state.new) {
				pvc_carrier(1, pvc);
				pvc->state.active = 1;
				fr_log_dlci_active(pvc);
			}

			if (lmi == LMI_CISCO) {
				data[i] = pvc->dlci >> 8;
				data[i + 1] = pvc->dlci & 0xFF;
			} else {
				data[i] = (pvc->dlci >> 4) & 0x3F;
				data[i + 1] = ((pvc->dlci << 3) & 0x78) | 0x80;
				data[i + 2] = 0x80;
			}

			if (pvc->state.new)
				data[i + 2] |= 0x08;
			else if (pvc->state.active)
				data[i + 2] |= 0x02;

			i += stat_len;
			pvc = pvc->next;
		}
	}

	skb_put(skb, i);
	skb->priority = TC_PRIO_CONTROL;
	skb->dev = dev;
	skb_reset_network_header(skb);

	dev_queue_xmit(skb);
}