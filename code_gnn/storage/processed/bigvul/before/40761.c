static int vmci_transport_recv_stream_cb(void *data, struct vmci_datagram *dg)
{
	struct sock *sk;
	struct sockaddr_vm dst;
	struct sockaddr_vm src;
	struct vmci_transport_packet *pkt;
	struct vsock_sock *vsk;
	bool bh_process_pkt;
	int err;

	sk = NULL;
	err = VMCI_SUCCESS;
	bh_process_pkt = false;

	 

	if (!vmci_transport_stream_allow(dg->src.context, -1)
	    || vmci_transport_peer_rid(dg->src.context) != dg->src.resource)
		return VMCI_ERROR_NO_ACCESS;

	if (VMCI_DG_SIZE(dg) < sizeof(*pkt))
		 
		return VMCI_ERROR_INVALID_ARGS;

	pkt = (struct vmci_transport_packet *)dg;

	 
	vsock_addr_init(&src, pkt->dg.src.context, pkt->src_port);
	vsock_addr_init(&dst, pkt->dg.dst.context, pkt->dst_port);

	sk = vsock_find_connected_socket(&src, &dst);
	if (!sk) {
		sk = vsock_find_bound_socket(&dst);
		if (!sk) {
			 
			if (vmci_transport_send_reset_bh(&dst, &src, pkt) < 0)
				pr_err("unable to send reset\n");

			err = VMCI_ERROR_NOT_FOUND;
			goto out;
		}
	}

	 
	if (pkt->type >= VMCI_TRANSPORT_PACKET_TYPE_MAX) {
		vmci_transport_send_invalid_bh(&dst, &src);
		err = VMCI_ERROR_INVALID_ARGS;
		goto out;
	}

	 
	vsk = vsock_sk(sk);
	if (!vmci_transport_allow_dgram(vsk, pkt->dg.src.context)) {
		err = VMCI_ERROR_NO_ACCESS;
		goto out;
	}

	 
	bh_lock_sock(sk);

	if (!sock_owned_by_user(sk)) {
		 
		vsk->local_addr.svm_cid = dst.svm_cid;

		if (sk->sk_state == SS_CONNECTED)
			vmci_trans(vsk)->notify_ops->handle_notify_pkt(
					sk, pkt, true, &dst, &src,
					&bh_process_pkt);
	}

	bh_unlock_sock(sk);

	if (!bh_process_pkt) {
		struct vmci_transport_recv_pkt_info *recv_pkt_info;

		recv_pkt_info = kmalloc(sizeof(*recv_pkt_info), GFP_ATOMIC);
		if (!recv_pkt_info) {
			if (vmci_transport_send_reset_bh(&dst, &src, pkt) < 0)
				pr_err("unable to send reset\n");

			err = VMCI_ERROR_NO_MEM;
			goto out;
		}

		recv_pkt_info->sk = sk;
		memcpy(&recv_pkt_info->pkt, pkt, sizeof(recv_pkt_info->pkt));
		INIT_WORK(&recv_pkt_info->work, vmci_transport_recv_pkt_work);

		schedule_work(&recv_pkt_info->work);
		 
		sk = NULL;
	}

out:
	if (sk)
		sock_put(sk);

	return err;
}
