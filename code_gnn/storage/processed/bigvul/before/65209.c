static int make_socks(struct svc_serv *serv, struct net *net)
{
	static int warned;
	int err;

	err = create_lockd_family(serv, net, PF_INET);
	if (err < 0)
		goto out_err;

	err = create_lockd_family(serv, net, PF_INET6);
	if (err < 0 && err != -EAFNOSUPPORT)
		goto out_err;

	warned = 0;
	return 0;

out_err:
	if (warned++ == 0)
		printk(KERN_WARNING
			"lockd_up: makesock failed, error=%d\n", err);
	svc_shutdown_net(serv, net);
	return err;
}
