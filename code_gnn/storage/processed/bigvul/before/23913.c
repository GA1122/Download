static int fr_lmi_recv(struct net_device *dev, struct sk_buff *skb)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	pvc_device *pvc;
	u8 rxseq, txseq;
	int lmi = state(hdlc)->settings.lmi;
	int dce = state(hdlc)->settings.dce;
	int stat_len = (lmi == LMI_CISCO) ? 6 : 3, reptype, error, no_ram, i;

	if (skb->len < (lmi == LMI_ANSI ? LMI_ANSI_LENGTH :
			LMI_CCITT_CISCO_LENGTH)) {
		netdev_info(dev, "Short LMI frame\n");
		return 1;
	}

	if (skb->data[3] != (lmi == LMI_CISCO ? NLPID_CISCO_LMI :
			     NLPID_CCITT_ANSI_LMI)) {
		netdev_info(dev, "Received non-LMI frame with LMI DLCI\n");
		return 1;
	}

	if (skb->data[4] != LMI_CALLREF) {
		netdev_info(dev, "Invalid LMI Call reference (0x%02X)\n",
			    skb->data[4]);
		return 1;
	}

	if (skb->data[5] != (dce ? LMI_STATUS_ENQUIRY : LMI_STATUS)) {
		netdev_info(dev, "Invalid LMI Message type (0x%02X)\n",
			    skb->data[5]);
		return 1;
	}

	if (lmi == LMI_ANSI) {
		if (skb->data[6] != LMI_ANSI_LOCKSHIFT) {
			netdev_info(dev, "Not ANSI locking shift in LMI message (0x%02X)\n",
				    skb->data[6]);
			return 1;
		}
		i = 7;
	} else
		i = 6;

	if (skb->data[i] != (lmi == LMI_CCITT ? LMI_CCITT_REPTYPE :
			     LMI_ANSI_CISCO_REPTYPE)) {
		netdev_info(dev, "Not an LMI Report type IE (0x%02X)\n",
			    skb->data[i]);
		return 1;
	}

	if (skb->data[++i] != LMI_REPT_LEN) {
		netdev_info(dev, "Invalid LMI Report type IE length (%u)\n",
			    skb->data[i]);
		return 1;
	}

	reptype = skb->data[++i];
	if (reptype != LMI_INTEGRITY && reptype != LMI_FULLREP) {
		netdev_info(dev, "Unsupported LMI Report type (0x%02X)\n",
			    reptype);
		return 1;
	}

	if (skb->data[++i] != (lmi == LMI_CCITT ? LMI_CCITT_ALIVE :
			       LMI_ANSI_CISCO_ALIVE)) {
		netdev_info(dev, "Not an LMI Link integrity verification IE (0x%02X)\n",
			    skb->data[i]);
		return 1;
	}

	if (skb->data[++i] != LMI_INTEG_LEN) {
		netdev_info(dev, "Invalid LMI Link integrity verification IE length (%u)\n",
			    skb->data[i]);
		return 1;
	}
	i++;

	state(hdlc)->rxseq = skb->data[i++];  
	rxseq = skb->data[i++];	 

	txseq = state(hdlc)->txseq;

	if (dce)
		state(hdlc)->last_poll = jiffies;

	error = 0;
	if (!state(hdlc)->reliable)
		error = 1;

	if (rxseq == 0 || rxseq != txseq) {  
		state(hdlc)->n391cnt = 0;
		error = 1;
	}

	if (dce) {
		if (state(hdlc)->fullrep_sent && !error) {
 
			state(hdlc)->fullrep_sent = 0;
			pvc = state(hdlc)->first_pvc;
			while (pvc) {
				if (pvc->state.new) {
					pvc->state.new = 0;

 
					state(hdlc)->dce_changed = 1;
				}
				pvc = pvc->next;
			}
		}

		if (state(hdlc)->dce_changed) {
			reptype = LMI_FULLREP;
			state(hdlc)->fullrep_sent = 1;
			state(hdlc)->dce_changed = 0;
		}

		state(hdlc)->request = 1;  
		fr_lmi_send(dev, reptype == LMI_FULLREP ? 1 : 0);
		return 0;
	}

	 

	state(hdlc)->request = 0;  

	if (error)
		return 0;

	if (reptype != LMI_FULLREP)
		return 0;

	pvc = state(hdlc)->first_pvc;

	while (pvc) {
		pvc->state.deleted = 1;
		pvc = pvc->next;
	}

	no_ram = 0;
	while (skb->len >= i + 2 + stat_len) {
		u16 dlci;
		u32 bw;
		unsigned int active, new;

		if (skb->data[i] != (lmi == LMI_CCITT ? LMI_CCITT_PVCSTAT :
				       LMI_ANSI_CISCO_PVCSTAT)) {
			netdev_info(dev, "Not an LMI PVC status IE (0x%02X)\n",
				    skb->data[i]);
			return 1;
		}

		if (skb->data[++i] != stat_len) {
			netdev_info(dev, "Invalid LMI PVC status IE length (%u)\n",
				    skb->data[i]);
			return 1;
		}
		i++;

		new = !! (skb->data[i + 2] & 0x08);
		active = !! (skb->data[i + 2] & 0x02);
		if (lmi == LMI_CISCO) {
			dlci = (skb->data[i] << 8) | skb->data[i + 1];
			bw = (skb->data[i + 3] << 16) |
				(skb->data[i + 4] << 8) |
				(skb->data[i + 5]);
		} else {
			dlci = ((skb->data[i] & 0x3F) << 4) |
				((skb->data[i + 1] & 0x78) >> 3);
			bw = 0;
		}

		pvc = add_pvc(dev, dlci);

		if (!pvc && !no_ram) {
			netdev_warn(dev, "Memory squeeze on fr_lmi_recv()\n");
			no_ram = 1;
		}

		if (pvc) {
			pvc->state.exist = 1;
			pvc->state.deleted = 0;
			if (active != pvc->state.active ||
			    new != pvc->state.new ||
			    bw != pvc->state.bandwidth ||
			    !pvc->state.exist) {
				pvc->state.new = new;
				pvc->state.active = active;
				pvc->state.bandwidth = bw;
				pvc_carrier(active, pvc);
				fr_log_dlci_active(pvc);
			}
		}

		i += stat_len;
	}

	pvc = state(hdlc)->first_pvc;

	while (pvc) {
		if (pvc->state.deleted && pvc->state.exist) {
			pvc_carrier(0, pvc);
			pvc->state.active = pvc->state.new = 0;
			pvc->state.exist = 0;
			pvc->state.bandwidth = 0;
			fr_log_dlci_active(pvc);
		}
		pvc = pvc->next;
	}

	 
	state(hdlc)->n391cnt = state(hdlc)->settings.n391;

	return 0;
}