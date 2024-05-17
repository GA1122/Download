vmci_transport_recv_connecting_server(struct sock *listener,
				      struct sock *pending,
				      struct vmci_transport_packet *pkt)
{
	struct vsock_sock *vpending;
	struct vmci_handle handle;
	struct vmci_qp *qpair;
	bool is_local;
	u32 flags;
	u32 detach_sub_id;
	int err;
	int skerr;

	vpending = vsock_sk(pending);
	detach_sub_id = VMCI_INVALID_ID;

	switch (pkt->type) {
	case VMCI_TRANSPORT_PACKET_TYPE_OFFER:
		if (vmci_handle_is_invalid(pkt->u.handle)) {
			vmci_transport_send_reset(pending, pkt);
			skerr = EPROTO;
			err = -EINVAL;
			goto destroy;
		}
		break;
	default:
		 
		vmci_transport_send_reset(pending, pkt);
		skerr = EPROTO;
		err = pkt->type == VMCI_TRANSPORT_PACKET_TYPE_RST ? 0 : -EINVAL;
		goto destroy;
	}

	 

	 
	err = vmci_event_subscribe(VMCI_EVENT_QP_PEER_DETACH,
				   vmci_transport_peer_detach_cb,
				   pending, &detach_sub_id);
	if (err < VMCI_SUCCESS) {
		vmci_transport_send_reset(pending, pkt);
		err = vmci_transport_error_to_vsock_error(err);
		skerr = -err;
		goto destroy;
	}

	vmci_trans(vpending)->detach_sub_id = detach_sub_id;

	 
	handle = pkt->u.handle;

	 
	is_local =
	    vpending->remote_addr.svm_cid == vpending->local_addr.svm_cid;
	flags = VMCI_QPFLAG_ATTACH_ONLY;
	flags |= is_local ? VMCI_QPFLAG_LOCAL : 0;

	err = vmci_transport_queue_pair_alloc(
					&qpair,
					&handle,
					vmci_trans(vpending)->produce_size,
					vmci_trans(vpending)->consume_size,
					pkt->dg.src.context,
					flags,
					vmci_transport_is_trusted(
						vpending,
						vpending->remote_addr.svm_cid));
	if (err < 0) {
		vmci_transport_send_reset(pending, pkt);
		skerr = -err;
		goto destroy;
	}

	vmci_trans(vpending)->qp_handle = handle;
	vmci_trans(vpending)->qpair = qpair;

	 
	pending->sk_state = SS_CONNECTED;

	vsock_insert_connected(vpending);

	 
	err = vmci_transport_send_attach(pending, handle);
	if (err < 0) {
		vsock_remove_connected(vpending);
		pr_err("Could not send attach\n");
		vmci_transport_send_reset(pending, pkt);
		err = vmci_transport_error_to_vsock_error(err);
		skerr = -err;
		goto destroy;
	}

	 
	vsock_remove_pending(listener, pending);
	vsock_enqueue_accept(listener, pending);

	 
	listener->sk_state_change(listener);

	return 0;

destroy:
	pending->sk_err = skerr;
	pending->sk_state = SS_UNCONNECTED;
	 
	sock_put(pending);

	return err;
}
