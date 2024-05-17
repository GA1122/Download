static void vmci_transport_handle_detach(struct sock *sk)
{
	struct vsock_sock *vsk;

	vsk = vsock_sk(sk);
	if (!vmci_handle_is_invalid(vmci_trans(vsk)->qp_handle)) {
		sock_set_flag(sk, SOCK_DONE);

		 
		vsk->peer_shutdown = SHUTDOWN_MASK;

		 
		if (vsock_stream_has_data(vsk) <= 0) {
			if (sk->sk_state == SS_CONNECTING) {
				 

				sk->sk_state = SS_UNCONNECTED;
				sk->sk_err = ECONNRESET;
				sk->sk_error_report(sk);
				return;
			}
			sk->sk_state = SS_UNCONNECTED;
		}
		sk->sk_state_change(sk);
	}
}
