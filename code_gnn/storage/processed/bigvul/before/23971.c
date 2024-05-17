static void airo_handle_rx(struct airo_info *ai)
{
	struct sk_buff *skb = NULL;
	__le16 fc, v, *buffer, tmpbuf[4];
	u16 len, hdrlen = 0, gap, fid;
	struct rx_hdr hdr;
	int success = 0;

	if (test_bit(FLAG_MPI, &ai->flags)) {
		if (test_bit(FLAG_802_11, &ai->flags))
			mpi_receive_802_11(ai);
		else
			mpi_receive_802_3(ai);
		OUT4500(ai, EVACK, EV_RX);
		return;
	}

	fid = IN4500(ai, RXFID);

	 
	if (test_bit(FLAG_802_11, &ai->flags)) {
		bap_setup (ai, fid, 4, BAP0);
		bap_read (ai, (__le16*)&hdr, sizeof(hdr), BAP0);
		 
		if (le16_to_cpu(hdr.status) & 2)
			hdr.len = 0;
		if (ai->wifidev == NULL)
			hdr.len = 0;
	} else {
		bap_setup(ai, fid, 0x36, BAP0);
		bap_read(ai, &hdr.len, 2, BAP0);
	}
	len = le16_to_cpu(hdr.len);

	if (len > AIRO_DEF_MTU) {
		airo_print_err(ai->dev->name, "Bad size %d", len);
		goto done;
	}
	if (len == 0)
		goto done;

	if (test_bit(FLAG_802_11, &ai->flags)) {
		bap_read(ai, &fc, sizeof (fc), BAP0);
		hdrlen = header_len(fc);
	} else
		hdrlen = ETH_ALEN * 2;

	skb = dev_alloc_skb(len + hdrlen + 2 + 2);
	if (!skb) {
		ai->dev->stats.rx_dropped++;
		goto done;
	}

	skb_reserve(skb, 2);  
	buffer = (__le16 *) skb_put(skb, len + hdrlen);
	if (test_bit(FLAG_802_11, &ai->flags)) {
		buffer[0] = fc;
		bap_read(ai, buffer + 1, hdrlen - 2, BAP0);
		if (hdrlen == 24)
			bap_read(ai, tmpbuf, 6, BAP0);

		bap_read(ai, &v, sizeof(v), BAP0);
		gap = le16_to_cpu(v);
		if (gap) {
			if (gap <= 8) {
				bap_read(ai, tmpbuf, gap, BAP0);
			} else {
				airo_print_err(ai->dev->name, "gaplen too "
					"big. Problems will follow...");
			}
		}
		bap_read(ai, buffer + hdrlen/2, len, BAP0);
	} else {
		MICBuffer micbuf;

		bap_read(ai, buffer, ETH_ALEN * 2, BAP0);
		if (ai->micstats.enabled) {
			bap_read(ai, (__le16 *) &micbuf, sizeof (micbuf), BAP0);
			if (ntohs(micbuf.typelen) > 0x05DC)
				bap_setup(ai, fid, 0x44, BAP0);
			else {
				if (len <= sizeof (micbuf)) {
					dev_kfree_skb_irq(skb);
					goto done;
				}

				len -= sizeof(micbuf);
				skb_trim(skb, len + hdrlen);
			}
		}

		bap_read(ai, buffer + ETH_ALEN, len, BAP0);
		if (decapsulate(ai, &micbuf, (etherHead*) buffer, len))
			dev_kfree_skb_irq (skb);
		else
			success = 1;
	}

#ifdef WIRELESS_SPY
	if (success && (ai->spy_data.spy_number > 0)) {
		char *sa;
		struct iw_quality wstats;

		 
		if (!test_bit(FLAG_802_11, &ai->flags)) {
			sa = (char *) buffer + 6;
			bap_setup(ai, fid, 8, BAP0);
			bap_read(ai, (__le16 *) hdr.rssi, 2, BAP0);
		} else
			sa = (char *) buffer + 10;
		wstats.qual = hdr.rssi[0];
		if (ai->rssi)
			wstats.level = 0x100 - ai->rssi[hdr.rssi[1]].rssidBm;
		else
			wstats.level = (hdr.rssi[1] + 321) / 2;
		wstats.noise = ai->wstats.qual.noise;
		wstats.updated =  IW_QUAL_LEVEL_UPDATED
				| IW_QUAL_QUAL_UPDATED
				| IW_QUAL_DBM;
		 
		wireless_spy_update(ai->dev, sa, &wstats);
	}
#endif  

done:
	OUT4500(ai, EVACK, EV_RX);

	if (success) {
		if (test_bit(FLAG_802_11, &ai->flags)) {
			skb_reset_mac_header(skb);
			skb->pkt_type = PACKET_OTHERHOST;
			skb->dev = ai->wifidev;
			skb->protocol = htons(ETH_P_802_2);
		} else
			skb->protocol = eth_type_trans(skb, ai->dev);
		skb->ip_summed = CHECKSUM_NONE;

		netif_rx(skb);
	}
}