static struct ethhdr *fm10k_port_is_vxlan(struct sk_buff *skb)
{
	struct fm10k_intfc *interface = netdev_priv(skb->dev);
	struct fm10k_udp_port *vxlan_port;

	 
	vxlan_port = list_first_entry_or_null(&interface->vxlan_port,
					      struct fm10k_udp_port, list);

	if (!vxlan_port)
		return NULL;
	if (vxlan_port->port != udp_hdr(skb)->dest)
		return NULL;

	 
	return (struct ethhdr *)(skb_transport_header(skb) + VXLAN_HLEN);
}
