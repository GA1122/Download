static int pppol2tp_session_create(struct net *net, u32 tunnel_id, u32 session_id, u32 peer_session_id, struct l2tp_session_cfg *cfg)
{
	int error;
	struct l2tp_tunnel *tunnel;
	struct l2tp_session *session;
	struct pppol2tp_session *ps;

	tunnel = l2tp_tunnel_find(net, tunnel_id);

	 
	error = -ENOENT;
	if (tunnel == NULL)
		goto out;

	 
	if (tunnel->sock == NULL)
		goto out;

	 
	error = -EEXIST;
	session = l2tp_session_find(net, tunnel, session_id);
	if (session != NULL)
		goto out;

	 
	if (cfg->mtu == 0)
		cfg->mtu = 1500 - PPPOL2TP_HEADER_OVERHEAD;
	if (cfg->mru == 0)
		cfg->mru = cfg->mtu;

	 
	error = -ENOMEM;
	session = l2tp_session_create(sizeof(struct pppol2tp_session),
				      tunnel, session_id,
				      peer_session_id, cfg);
	if (session == NULL)
		goto out;

	ps = l2tp_session_priv(session);
	ps->tunnel_sock = tunnel->sock;

	l2tp_info(session, PPPOL2TP_MSG_CONTROL, "%s: created\n",
		  session->name);

	error = 0;

out:
	return error;
}