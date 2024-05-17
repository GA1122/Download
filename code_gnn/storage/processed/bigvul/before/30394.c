static int vmci_transport_notify_send_pre_block(
	struct vsock_sock *vsk,
	struct vsock_transport_send_notify_data *data)
{
	return vmci_trans(vsk)->notify_ops->send_pre_block(
			&vsk->sk,
			(struct vmci_transport_send_notify_data *)data);
}