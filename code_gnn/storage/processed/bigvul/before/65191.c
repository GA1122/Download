static int create_lockd_listener(struct svc_serv *serv, const char *name,
				 struct net *net, const int family,
				 const unsigned short port)
{
	struct svc_xprt *xprt;

	xprt = svc_find_xprt(serv, name, net, family, 0);
	if (xprt == NULL)
		return svc_create_xprt(serv, name, net, family, port,
						SVC_SOCK_DEFAULTS);
	svc_xprt_put(xprt);
	return 0;
}
