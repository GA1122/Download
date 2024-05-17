static int pppol2tp_tunnel_ioctl(struct l2tp_tunnel *tunnel,
				 unsigned int cmd, unsigned long arg)
{
	int err = 0;
	struct sock *sk;
	struct pppol2tp_ioc_stats stats;

	l2tp_dbg(tunnel, PPPOL2TP_MSG_CONTROL,
		 "%s: pppol2tp_tunnel_ioctl(cmd=%#x, arg=%#lx)\n",
		 tunnel->name, cmd, arg);

	sk = tunnel->sock;
	sock_hold(sk);

	switch (cmd) {
	case PPPIOCGL2TPSTATS:
		err = -ENXIO;
		if (!(sk->sk_state & PPPOX_CONNECTED))
			break;

		if (copy_from_user(&stats, (void __user *) arg,
				   sizeof(stats))) {
			err = -EFAULT;
			break;
		}
		if (stats.session_id != 0) {
			 
			struct l2tp_session *session =
				l2tp_session_find(sock_net(sk), tunnel, stats.session_id);
			if (session != NULL)
				err = pppol2tp_session_ioctl(session, cmd, arg);
			else
				err = -EBADR;
			break;
		}
#ifdef CONFIG_XFRM
		stats.using_ipsec = (sk->sk_policy[0] || sk->sk_policy[1]) ? 1 : 0;
#endif
		pppol2tp_copy_stats(&stats, &tunnel->stats);
		if (copy_to_user((void __user *) arg, &stats, sizeof(stats))) {
			err = -EFAULT;
			break;
		}
		l2tp_info(tunnel, PPPOL2TP_MSG_CONTROL, "%s: get L2TP stats\n",
			  tunnel->name);
		err = 0;
		break;

	default:
		err = -ENOSYS;
		break;
	}

	sock_put(sk);

	return err;
}