static void vsock_sk_destruct(struct sock *sk)
{
	struct vsock_sock *vsk = vsock_sk(sk);

	transport->destruct(vsk);

	 
	vsock_addr_init(&vsk->local_addr, VMADDR_CID_ANY, VMADDR_PORT_ANY);
	vsock_addr_init(&vsk->remote_addr, VMADDR_CID_ANY, VMADDR_PORT_ANY);

	put_cred(vsk->owner);
}
