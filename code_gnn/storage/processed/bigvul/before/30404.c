static int vmci_transport_recv_connected(struct sock *sk,
					 struct vmci_transport_packet *pkt)
{
	struct vsock_sock *vsk;
	bool pkt_processed = false;

	 
	switch (pkt->type) {
	case VMCI_TRANSPORT_PACKET_TYPE_SHUTDOWN:
		if (pkt->u.mode) {
			vsk = vsock_sk(sk);

			vsk->peer_shutdown |= pkt->u.mode;
			sk->sk_state_change(sk);
		}
		break;

	case VMCI_TRANSPORT_PACKET_TYPE_RST:
		vsk = vsock_sk(sk);
		 
		sock_set_flag(sk, SOCK_DONE);
		vsk->peer_shutdown = SHUTDOWN_MASK;
		if (vsock_stream_has_data(vsk) <= 0)
			sk->sk_state = SS_DISCONNECTING;

		sk->sk_state_change(sk);
		break;

	default:
		vsk = vsock_sk(sk);
		vmci_trans(vsk)->notify_ops->handle_notify_pkt(
				sk, pkt, false, NULL, NULL,
				&pkt_processed);
		if (!pkt_processed)
			return -EINVAL;

		break;
	}

	return 0;
}
