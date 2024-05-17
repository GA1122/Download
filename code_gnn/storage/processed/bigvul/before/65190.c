static int create_lockd_family(struct svc_serv *serv, struct net *net,
			       const int family)
{
	int err;

	err = create_lockd_listener(serv, "udp", net, family, nlm_udpport);
	if (err < 0)
		return err;

	return create_lockd_listener(serv, "tcp", net, family, nlm_tcpport);
}
