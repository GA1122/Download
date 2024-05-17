static int __init l2tp_eth_init(void)
{
	int err = 0;

	err = l2tp_nl_register_ops(L2TP_PWTYPE_ETH, &l2tp_eth_nl_cmd_ops);
	if (err)
		goto out;

	err = register_pernet_device(&l2tp_eth_net_ops);
	if (err)
		goto out_unreg;

	printk(KERN_INFO "L2TP ethernet pseudowire support (L2TPv3)\n");

	return 0;

out_unreg:
	l2tp_nl_unregister_ops(L2TP_PWTYPE_ETH);
out:
	return err;
}