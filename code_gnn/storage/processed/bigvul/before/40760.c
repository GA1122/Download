static void vmci_transport_recv_pkt_work(struct work_struct *work)
{
	struct vmci_transport_recv_pkt_info *recv_pkt_info;
	struct vmci_transport_packet *pkt;
	struct sock *sk;

	recv_pkt_info =
		container_of(work, struct vmci_transport_recv_pkt_info, work);
	sk = recv_pkt_info->sk;
	pkt = &recv_pkt_info->pkt;

	lock_sock(sk);

	 
	vsock_sk(sk)->local_addr.svm_cid = pkt->dg.dst.context;

	switch (sk->sk_state) {
	case SS_LISTEN:
		vmci_transport_recv_listen(sk, pkt);
		break;
	case SS_CONNECTING:
		 
		vmci_transport_recv_connecting_client(sk, pkt);
		break;
	case SS_CONNECTED:
		vmci_transport_recv_connected(sk, pkt);
		break;
	default:
		 
		vmci_transport_send_reset(sk, pkt);
		break;
	}

	release_sock(sk);
	kfree(recv_pkt_info);
	 
	sock_put(sk);
}
