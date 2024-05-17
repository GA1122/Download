static int vsock_getname(struct socket *sock,
			 struct sockaddr *addr, int *addr_len, int peer)
{
	int err;
	struct sock *sk;
	struct vsock_sock *vsk;
	struct sockaddr_vm *vm_addr;

	sk = sock->sk;
	vsk = vsock_sk(sk);
	err = 0;

	lock_sock(sk);

	if (peer) {
		if (sock->state != SS_CONNECTED) {
			err = -ENOTCONN;
			goto out;
		}
		vm_addr = &vsk->remote_addr;
	} else {
		vm_addr = &vsk->local_addr;
	}

	if (!vm_addr) {
		err = -EINVAL;
		goto out;
	}

	 
	BUILD_BUG_ON(sizeof(*vm_addr) > 128);
	memcpy(addr, vm_addr, sizeof(*vm_addr));
	*addr_len = sizeof(*vm_addr);

out:
	release_sock(sk);
	return err;
}
