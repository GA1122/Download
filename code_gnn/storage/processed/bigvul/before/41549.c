static int __init ipv4_offload_init(void)
{
	 
	if (udpv4_offload_init() < 0)
		pr_crit("%s: Cannot add UDP protocol offload\n", __func__);
	if (tcpv4_offload_init() < 0)
		pr_crit("%s: Cannot add TCP protocol offload\n", __func__);

	dev_add_offload(&ip_packet_offload);
	inet_add_offload(&ipip_offload, IPPROTO_IPIP);
	return 0;
}