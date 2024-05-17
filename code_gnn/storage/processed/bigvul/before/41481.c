static void dn_destroy_sock(struct sock *sk)
{
	struct dn_scp *scp = DN_SK(sk);

	scp->nsp_rxtshift = 0;  

	if (sk->sk_socket) {
		if (sk->sk_socket->state != SS_UNCONNECTED)
			sk->sk_socket->state = SS_DISCONNECTING;
	}

	sk->sk_state = TCP_CLOSE;

	switch (scp->state) {
	case DN_DN:
		dn_nsp_send_disc(sk, NSP_DISCCONF, NSP_REASON_DC,
				 sk->sk_allocation);
		scp->persist_fxn = dn_destroy_timer;
		scp->persist = dn_nsp_persist(sk);
		break;
	case DN_CR:
		scp->state = DN_DR;
		goto disc_reject;
	case DN_RUN:
		scp->state = DN_DI;
	case DN_DI:
	case DN_DR:
disc_reject:
		dn_nsp_send_disc(sk, NSP_DISCINIT, 0, sk->sk_allocation);
	case DN_NC:
	case DN_NR:
	case DN_RJ:
	case DN_DIC:
	case DN_CN:
	case DN_DRC:
	case DN_CI:
	case DN_CD:
		scp->persist_fxn = dn_destroy_timer;
		scp->persist = dn_nsp_persist(sk);
		break;
	default:
		printk(KERN_DEBUG "DECnet: dn_destroy_sock passed socket in invalid state\n");
	case DN_O:
		dn_stop_slow_timer(sk);

		dn_unhash_sock_bh(sk);
		sock_put(sk);

		break;
	}
}