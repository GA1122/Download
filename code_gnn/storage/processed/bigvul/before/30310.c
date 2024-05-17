static int __vsock_bind(struct sock *sk, struct sockaddr_vm *addr)
{
	struct vsock_sock *vsk = vsock_sk(sk);
	u32 cid;
	int retval;

	 
	if (vsock_addr_bound(&vsk->local_addr))
		return -EINVAL;

	 
	cid = transport->get_local_cid();
	if (addr->svm_cid != cid && addr->svm_cid != VMADDR_CID_ANY)
		return -EADDRNOTAVAIL;

	switch (sk->sk_socket->type) {
	case SOCK_STREAM:
		spin_lock_bh(&vsock_table_lock);
		retval = __vsock_bind_stream(vsk, addr);
		spin_unlock_bh(&vsock_table_lock);
		break;

	case SOCK_DGRAM:
		retval = __vsock_bind_dgram(vsk, addr);
		break;

	default:
		retval = -EINVAL;
		break;
	}

	return retval;
}
