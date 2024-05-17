static void ctdb_node_connect_write(struct event_context *ev, struct fd_event *fde, 
				    uint16_t flags, void *private_data)
{
	struct ctdb_node *node = talloc_get_type(private_data,
						 struct ctdb_node);
	struct ctdb_tcp_node *tnode = talloc_get_type(node->private_data,
						      struct ctdb_tcp_node);
	struct ctdb_context *ctdb = node->ctdb;
	int error = 0;
	socklen_t len = sizeof(error);
	int one = 1;

	talloc_free(tnode->connect_te);
	tnode->connect_te = NULL;

	if (getsockopt(tnode->fd, SOL_SOCKET, SO_ERROR, &error, &len) != 0 ||
	    error != 0) {
		ctdb_tcp_stop_connection(node);
		tnode->connect_te = event_add_timed(ctdb->ev, tnode, 
						    timeval_current_ofs(1, 0),
						    ctdb_tcp_node_connect, node);
		return;
	}

	talloc_free(tnode->connect_fde);
	tnode->connect_fde = NULL;

        setsockopt(tnode->fd,IPPROTO_TCP,TCP_NODELAY,(char *)&one,sizeof(one));
        setsockopt(tnode->fd,SOL_SOCKET,SO_KEEPALIVE,(char *)&one,sizeof(one));

	ctdb_queue_set_fd(tnode->out_queue, tnode->fd);

	 
	tnode->fd = -1;
}
