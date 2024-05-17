static int mpi_send_packet (struct net_device *dev)
{
	struct sk_buff *skb;
	unsigned char *buffer;
	s16 len;
	__le16 *payloadLen;
	struct airo_info *ai = dev->ml_priv;
	u8 *sendbuf;

	 

	if ((skb = skb_dequeue(&ai->txq)) == NULL) {
		airo_print_err(dev->name,
			"%s: Dequeue'd zero in send_packet()",
			__func__);
		return 0;
	}

	 
	len = ETH_ZLEN < skb->len ? skb->len : ETH_ZLEN;
	buffer = skb->data;

	ai->txfids[0].tx_desc.offset = 0;
	ai->txfids[0].tx_desc.valid = 1;
	ai->txfids[0].tx_desc.eoc = 1;
	ai->txfids[0].tx_desc.len =len+sizeof(WifiHdr);

 

	memcpy((char *)ai->txfids[0].virtual_host_addr,
		(char *)&wifictlhdr8023, sizeof(wifictlhdr8023));

	payloadLen = (__le16 *)(ai->txfids[0].virtual_host_addr +
		sizeof(wifictlhdr8023));
	sendbuf = ai->txfids[0].virtual_host_addr +
		sizeof(wifictlhdr8023) + 2 ;

	 
	if (test_bit(FLAG_MIC_CAPABLE, &ai->flags) && ai->micstats.enabled &&
		(ntohs(((__be16 *)buffer)[6]) != 0x888E)) {
		MICBuffer pMic;

		if (encapsulate(ai, (etherHead *)buffer, &pMic, len - sizeof(etherHead)) != SUCCESS)
			return ERROR;

		*payloadLen = cpu_to_le16(len-sizeof(etherHead)+sizeof(pMic));
		ai->txfids[0].tx_desc.len += sizeof(pMic);
		 
		memcpy (sendbuf, buffer, sizeof(etherHead));
		buffer += sizeof(etherHead);
		sendbuf += sizeof(etherHead);
		memcpy (sendbuf, &pMic, sizeof(pMic));
		sendbuf += sizeof(pMic);
		memcpy (sendbuf, buffer, len - sizeof(etherHead));
	} else {
		*payloadLen = cpu_to_le16(len - sizeof(etherHead));

		dev->trans_start = jiffies;

		 
		memcpy(sendbuf, buffer, len);
	}

	memcpy_toio(ai->txfids[0].card_ram_off,
		&ai->txfids[0].tx_desc, sizeof(TxFid));

	OUT4500(ai, EVACK, 8);

	dev_kfree_skb_any(skb);
	return 1;
}
