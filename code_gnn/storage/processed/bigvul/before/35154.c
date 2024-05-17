ipv6_connect(struct TCP_Server_Info *server)
{
	int rc = 0;
	int val;
	bool connected = false;
	__be16 orig_port = 0;
	struct socket *socket = server->ssocket;

	if (socket == NULL) {
		rc = sock_create_kern(PF_INET6, SOCK_STREAM,
				      IPPROTO_TCP, &socket);
		if (rc < 0) {
			cERROR(1, "Error %d creating ipv6 socket", rc);
			socket = NULL;
			return rc;
		}

		 
		cFYI(1, "ipv6 Socket created");
		server->ssocket = socket;
		socket->sk->sk_allocation = GFP_NOFS;
		cifs_reclassify_socket6(socket);
	}

	 
	if (server->addr.sockAddr6.sin6_port) {
		rc = socket->ops->connect(socket,
				(struct sockaddr *) &server->addr.sockAddr6,
				sizeof(struct sockaddr_in6), 0);
		if (rc >= 0)
			connected = true;
	}

	if (!connected) {
		 

		orig_port = server->addr.sockAddr6.sin6_port;
		 
		if (server->addr.sockAddr6.sin6_port != htons(CIFS_PORT)) {
			server->addr.sockAddr6.sin6_port = htons(CIFS_PORT);
			rc = socket->ops->connect(socket, (struct sockaddr *)
					&server->addr.sockAddr6,
					sizeof(struct sockaddr_in6), 0);
			if (rc >= 0)
				connected = true;
		}
	}
	if (!connected) {
		server->addr.sockAddr6.sin6_port = htons(RFC1001_PORT);
		rc = socket->ops->connect(socket, (struct sockaddr *)
				&server->addr.sockAddr6,
				sizeof(struct sockaddr_in6), 0);
		if (rc >= 0)
			connected = true;
	}

	 
	if (!connected) {
		if (orig_port)
			server->addr.sockAddr6.sin6_port = orig_port;
		cFYI(1, "Error %d connecting to server via ipv6", rc);
		sock_release(socket);
		server->ssocket = NULL;
		return rc;
	}

	 
	socket->sk->sk_rcvtimeo = 7 * HZ;
	socket->sk->sk_sndtimeo = 5 * HZ;

	if (server->tcp_nodelay) {
		val = 1;
		rc = kernel_setsockopt(socket, SOL_TCP, TCP_NODELAY,
				(char *)&val, sizeof(val));
		if (rc)
			cFYI(1, "set TCP_NODELAY socket option error %d", rc);
	}

	server->ssocket = socket;

	return rc;
}