static void mpi_receive_802_3(struct airo_info *ai)
{
	RxFid rxd;
	int len = 0;
	struct sk_buff *skb;
	char *buffer;
	int off = 0;
	MICBuffer micbuf;

	memcpy_fromio(&rxd, ai->rxfids[0].card_ram_off, sizeof(rxd));
	 
	if (rxd.rdy && rxd.valid == 0) {
		len = rxd.len + 12;
		if (len < 12 || len > 2048)
			goto badrx;

		skb = dev_alloc_skb(len);
		if (!skb) {
			ai->dev->stats.rx_dropped++;
			goto badrx;
		}
		buffer = skb_put(skb,len);
		memcpy(buffer, ai->rxfids[0].virtual_host_addr, ETH_ALEN * 2);
		if (ai->micstats.enabled) {
			memcpy(&micbuf,
				ai->rxfids[0].virtual_host_addr + ETH_ALEN * 2,
				sizeof(micbuf));
			if (ntohs(micbuf.typelen) <= 0x05DC) {
				if (len <= sizeof(micbuf) + ETH_ALEN * 2)
					goto badmic;

				off = sizeof(micbuf);
				skb_trim (skb, len - off);
			}
		}
		memcpy(buffer + ETH_ALEN * 2,
			ai->rxfids[0].virtual_host_addr + ETH_ALEN * 2 + off,
			len - ETH_ALEN * 2 - off);
		if (decapsulate (ai, &micbuf, (etherHead*)buffer, len - off - ETH_ALEN * 2)) {
badmic:
			dev_kfree_skb_irq (skb);
			goto badrx;
		}
#ifdef WIRELESS_SPY
		if (ai->spy_data.spy_number > 0) {
			char *sa;
			struct iw_quality wstats;
			 
			sa = buffer + ETH_ALEN;
			wstats.qual = 0;  
			wstats.level = 0;
			wstats.updated = 0;
			 
			wireless_spy_update(ai->dev, sa, &wstats);
		}
#endif  

		skb->ip_summed = CHECKSUM_NONE;
		skb->protocol = eth_type_trans(skb, ai->dev);
		netif_rx(skb);
	}
badrx:
	if (rxd.valid == 0) {
		rxd.valid = 1;
		rxd.rdy = 0;
		rxd.len = PKTSIZE;
		memcpy_toio(ai->rxfids[0].card_ram_off, &rxd, sizeof(rxd));
	}
}