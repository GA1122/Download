static void __exit l2tp_eth_exit(void)
{
	unregister_pernet_device(&l2tp_eth_net_ops);
	l2tp_nl_unregister_ops(L2TP_PWTYPE_ETH);
}