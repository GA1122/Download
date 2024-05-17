int svc_register(const struct svc_serv *serv, struct net *net,
		 const int family, const unsigned short proto,
		 const unsigned short port)
{
	struct svc_program	*progp;
	struct svc_version	*vers;
	unsigned int		i;
	int			error = 0;

	WARN_ON_ONCE(proto == 0 && port == 0);
	if (proto == 0 && port == 0)
		return -EINVAL;

	for (progp = serv->sv_program; progp; progp = progp->pg_next) {
		for (i = 0; i < progp->pg_nvers; i++) {
			vers = progp->pg_vers[i];
			if (vers == NULL)
				continue;

			dprintk("svc: svc_register(%sv%d, %s, %u, %u)%s\n",
					progp->pg_name,
					i,
					proto == IPPROTO_UDP?  "udp" : "tcp",
					port,
					family,
					vers->vs_hidden ?
					" (but not telling portmap)" : "");

			if (vers->vs_hidden)
				continue;

			 
			if (vers->vs_need_cong_ctrl && proto == IPPROTO_UDP)
				continue;

			error = __svc_register(net, progp->pg_name, progp->pg_prog,
						i, family, proto, port);

			if (vers->vs_rpcb_optnl) {
				error = 0;
				continue;
			}

			if (error < 0) {
				printk(KERN_WARNING "svc: failed to register "
					"%sv%u RPC service (errno %d).\n",
					progp->pg_name, i, -error);
				break;
			}
		}
	}

	return error;
}