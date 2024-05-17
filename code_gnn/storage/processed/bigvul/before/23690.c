static void bond_arp_rcv(struct sk_buff *skb, struct bonding *bond,
			 struct slave *slave)
{
	struct arphdr *arp;
	unsigned char *arp_ptr;
	__be32 sip, tip;

	if (skb->protocol != __cpu_to_be16(ETH_P_ARP))
		return;

	read_lock(&bond->lock);

	pr_debug("bond_arp_rcv: bond %s skb->dev %s\n",
		 bond->dev->name, skb->dev->name);

	if (!pskb_may_pull(skb, arp_hdr_len(bond->dev)))
		goto out_unlock;

	arp = arp_hdr(skb);
	if (arp->ar_hln != bond->dev->addr_len ||
	    skb->pkt_type == PACKET_OTHERHOST ||
	    skb->pkt_type == PACKET_LOOPBACK ||
	    arp->ar_hrd != htons(ARPHRD_ETHER) ||
	    arp->ar_pro != htons(ETH_P_IP) ||
	    arp->ar_pln != 4)
		goto out_unlock;

	arp_ptr = (unsigned char *)(arp + 1);
	arp_ptr += bond->dev->addr_len;
	memcpy(&sip, arp_ptr, 4);
	arp_ptr += 4 + bond->dev->addr_len;
	memcpy(&tip, arp_ptr, 4);

	pr_debug("bond_arp_rcv: %s %s/%d av %d sv %d sip %pI4 tip %pI4\n",
		 bond->dev->name, slave->dev->name, bond_slave_state(slave),
		 bond->params.arp_validate, slave_do_arp_validate(bond, slave),
		 &sip, &tip);

	 
	if (bond_is_active_slave(slave))
		bond_validate_arp(bond, slave, sip, tip);
	else
		bond_validate_arp(bond, slave, tip, sip);

out_unlock:
	read_unlock(&bond->lock);
}