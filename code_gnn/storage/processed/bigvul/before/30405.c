vmci_transport_recv_connecting_client(struct sock *sk,
				      struct vmci_transport_packet *pkt)
{
	struct vsock_sock *vsk;
	int err;
	int skerr;

	vsk = vsock_sk(sk);

	switch (pkt->type) {
	case VMCI_TRANSPORT_PACKET_TYPE_ATTACH:
		if (vmci_handle_is_invalid(pkt->u.handle) ||
		    !vmci_handle_is_equal(pkt->u.handle,
					  vmci_trans(vsk)->qp_handle)) {
			skerr = EPROTO;
			err = -EINVAL;
			goto destroy;
		}

		 
		sk->sk_state = SS_CONNECTED;
		sk->sk_socket->state = SS_CONNECTED;
		vsock_insert_connected(vsk);
		sk->sk_state_change(sk);

		break;
	case VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE:
	case VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE2:
		if (pkt->u.size == 0
		    || pkt->dg.src.context != vsk->remote_addr.svm_cid
		    || pkt->src_port != vsk->remote_addr.svm_port
		    || !vmci_handle_is_invalid(vmci_trans(vsk)->qp_handle)
		    || vmci_trans(vsk)->qpair
		    || vmci_trans(vsk)->produce_size != 0
		    || vmci_trans(vsk)->consume_size != 0
		    || vmci_trans(vsk)->attach_sub_id != VMCI_INVALID_ID
		    || vmci_trans(vsk)->detach_sub_id != VMCI_INVALID_ID) {
			skerr = EPROTO;
			err = -EINVAL;

			goto destroy;
		}

		err = vmci_transport_recv_connecting_client_negotiate(sk, pkt);
		if (err) {
			skerr = -err;
			goto destroy;
		}

		break;
	case VMCI_TRANSPORT_PACKET_TYPE_INVALID:
		err = vmci_transport_recv_connecting_client_invalid(sk, pkt);
		if (err) {
			skerr = -err;
			goto destroy;
		}

		break;
	case VMCI_TRANSPORT_PACKET_TYPE_RST:
		 
		if (vsk->ignore_connecting_rst) {
			vsk->ignore_connecting_rst = false;
		} else {
			skerr = ECONNRESET;
			err = 0;
			goto destroy;
		}

		break;
	default:
		 
		skerr = EPROTO;
		err = -EINVAL;
		goto destroy;
	}

	return 0;

destroy:
	vmci_transport_send_reset(sk, pkt);

	sk->sk_state = SS_UNCONNECTED;
	sk->sk_err = skerr;
	sk->sk_error_report(sk);
	return err;
}