static bool filter_connect(struct tipc_sock *tsk, struct sk_buff *skb)
{
	struct sock *sk = &tsk->sk;
	struct net *net = sock_net(sk);
	struct socket *sock = sk->sk_socket;
	struct tipc_msg *hdr = buf_msg(skb);

	if (unlikely(msg_mcast(hdr)))
		return false;

	switch ((int)sock->state) {
	case SS_CONNECTED:

		 
		if (unlikely(!tsk_peer_msg(tsk, hdr)))
			return false;

		if (unlikely(msg_errcode(hdr))) {
			sock->state = SS_DISCONNECTING;
			tsk->connected = 0;
			 
			tipc_node_remove_conn(net, tsk_peer_node(tsk),
					      tsk->portid);
		}
		return true;

	case SS_CONNECTING:

		 
		if (unlikely(!msg_connected(hdr)))
			return false;

		if (unlikely(msg_errcode(hdr))) {
			sock->state = SS_DISCONNECTING;
			sk->sk_err = ECONNREFUSED;
			return true;
		}

		if (unlikely(!msg_isdata(hdr))) {
			sock->state = SS_DISCONNECTING;
			sk->sk_err = EINVAL;
			return true;
		}

		tipc_sk_finish_conn(tsk, msg_origport(hdr), msg_orignode(hdr));
		msg_set_importance(&tsk->phdr, msg_importance(hdr));
		sock->state = SS_CONNECTED;

		 
		if (msg_data_sz(hdr))
			return true;

		 
		if (waitqueue_active(sk_sleep(sk)))
			wake_up_interruptible(sk_sleep(sk));

		 
		msg_set_dest_droppable(hdr, 1);
		return false;

	case SS_LISTENING:
	case SS_UNCONNECTED:

		 
		if (!msg_connected(hdr) && !(msg_errcode(hdr)))
			return true;
		break;
	case SS_DISCONNECTING:
		break;
	default:
		pr_err("Unknown socket state %u\n", sock->state);
	}
	return false;
}