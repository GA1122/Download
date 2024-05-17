void nf_nat_masquerade_ipv4_register_notifier(void)
{
	 
	if (atomic_inc_return(&masquerade_notifier_refcount) > 1)
		return;

	 
	register_netdevice_notifier(&masq_dev_notifier);
	 
	register_inetaddr_notifier(&masq_inet_notifier);
}
