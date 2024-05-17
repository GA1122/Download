static struct sock *dn_alloc_sock(struct net *net, struct socket *sock, gfp_t gfp, int kern)
{
	struct dn_scp *scp;
	struct sock *sk = sk_alloc(net, PF_DECnet, gfp, &dn_proto, kern);

	if  (!sk)
		goto out;

	if (sock)
		sock->ops = &dn_proto_ops;
	sock_init_data(sock, sk);

	sk->sk_backlog_rcv = dn_nsp_backlog_rcv;
	sk->sk_destruct    = dn_destruct;
	sk->sk_no_check_tx = 1;
	sk->sk_family      = PF_DECnet;
	sk->sk_protocol    = 0;
	sk->sk_allocation  = gfp;
	sk->sk_sndbuf	   = sysctl_decnet_wmem[1];
	sk->sk_rcvbuf	   = sysctl_decnet_rmem[1];

	 
	scp = DN_SK(sk);
	scp->state	= DN_O;		 
	scp->numdat	= 1;		 
	scp->numoth	= 1;		 
	scp->ackxmt_dat = 0;		 
	scp->ackxmt_oth = 0;		 
	scp->ackrcv_dat = 0;		 
	scp->ackrcv_oth = 0;		 
	scp->flowrem_sw = DN_SEND;
	scp->flowloc_sw = DN_SEND;
	scp->flowrem_dat = 0;
	scp->flowrem_oth = 1;
	scp->flowloc_dat = 0;
	scp->flowloc_oth = 1;
	scp->services_rem = 0;
	scp->services_loc = 1 | NSP_FC_NONE;
	scp->info_rem = 0;
	scp->info_loc = 0x03;  
	scp->segsize_rem = 230 - DN_MAX_NSP_DATA_HEADER;  
	scp->nonagle = 0;
	scp->multi_ireq = 1;
	scp->accept_mode = ACC_IMMED;
	scp->addr.sdn_family    = AF_DECnet;
	scp->peer.sdn_family    = AF_DECnet;
	scp->accessdata.acc_accl = 5;
	memcpy(scp->accessdata.acc_acc, "LINUX", 5);

	scp->max_window   = NSP_MAX_WINDOW;
	scp->snd_window   = NSP_MIN_WINDOW;
	scp->nsp_srtt     = NSP_INITIAL_SRTT;
	scp->nsp_rttvar   = NSP_INITIAL_RTTVAR;
	scp->nsp_rxtshift = 0;

	skb_queue_head_init(&scp->data_xmit_queue);
	skb_queue_head_init(&scp->other_xmit_queue);
	skb_queue_head_init(&scp->other_receive_queue);

	scp->persist = 0;
	scp->persist_fxn = NULL;
	scp->keepalive = 10 * HZ;
	scp->keepalive_fxn = dn_keepalive;

	init_timer(&scp->delack_timer);
	scp->delack_pending = 0;
	scp->delack_fxn = dn_nsp_delayed_ack;

	dn_start_slow_timer(sk);
out:
	return sk;
}