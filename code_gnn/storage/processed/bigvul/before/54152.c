void nf_nat_masquerade_ipv4_unregister_notifier(void)
{
	 
	if (atomic_dec_return(&masquerade_notifier_refcount) > 0)
		return;

	unregister_netdevice_notifier(&masq_dev_notifier);
	unregister_inetaddr_notifier(&masq_inet_notifier);
}