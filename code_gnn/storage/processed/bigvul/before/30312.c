static int __vsock_bind_stream(struct vsock_sock *vsk,
			       struct sockaddr_vm *addr)
{
	static u32 port = LAST_RESERVED_PORT + 1;
	struct sockaddr_vm new_addr;

	vsock_addr_init(&new_addr, addr->svm_cid, addr->svm_port);

	if (addr->svm_port == VMADDR_PORT_ANY) {
		bool found = false;
		unsigned int i;

		for (i = 0; i < MAX_PORT_RETRIES; i++) {
			if (port <= LAST_RESERVED_PORT)
				port = LAST_RESERVED_PORT + 1;

			new_addr.svm_port = port++;

			if (!__vsock_find_bound_socket(&new_addr)) {
				found = true;
				break;
			}
		}

		if (!found)
			return -EADDRNOTAVAIL;
	} else {
		 
		if (addr->svm_port <= LAST_RESERVED_PORT &&
		    !capable(CAP_NET_BIND_SERVICE)) {
			return -EACCES;
		}

		if (__vsock_find_bound_socket(&new_addr))
			return -EADDRINUSE;
	}

	vsock_addr_init(&vsk->local_addr, new_addr.svm_cid, new_addr.svm_port);

	 
	__vsock_remove_bound(vsk);
	__vsock_insert_bound(vsock_bound_sockets(&vsk->local_addr), vsk);

	return 0;
}
