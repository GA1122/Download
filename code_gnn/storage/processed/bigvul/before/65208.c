static int lockd_up_net(struct svc_serv *serv, struct net *net)
{
	struct lockd_net *ln = net_generic(net, lockd_net_id);
	int error;

	if (ln->nlmsvc_users++)
		return 0;

	error = svc_bind(serv, net);
	if (error)
		goto err_bind;

	error = make_socks(serv, net);
	if (error < 0)
		goto err_bind;
	set_grace_period(net);
	dprintk("lockd_up_net: per-net data created; net=%p\n", net);
	return 0;

err_bind:
	ln->nlmsvc_users--;
	return error;
}
