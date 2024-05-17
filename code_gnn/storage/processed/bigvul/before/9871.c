int conn_si_send_proxy(struct connection *conn, unsigned int flag)
{
	 
	if (conn->flags & CO_FL_SOCK_WR_SH)
		goto out_error;

	if (!conn_ctrl_ready(conn))
		goto out_error;

	if (!fd_send_ready(conn->t.sock.fd))
		goto out_wait;

	 
	while (conn->send_proxy_ofs) {
		int ret;

		 
		if (conn->data == &si_conn_cb) {
			struct stream_interface *si = conn->owner;
			struct connection *remote = objt_conn(si->ob->prod->end);
			ret = make_proxy_line(trash.str, trash.size, objt_server(conn->target), remote);
		}
		else {
			 
			conn_get_from_addr(conn);
			if (!(conn->flags & CO_FL_ADDR_FROM_SET))
				goto out_wait;

			conn_get_to_addr(conn);
			if (!(conn->flags & CO_FL_ADDR_TO_SET))
				goto out_wait;

			ret = make_proxy_line(trash.str, trash.size, objt_server(conn->target), conn);
		}

		if (!ret)
			goto out_error;

		if (conn->send_proxy_ofs > 0)
			conn->send_proxy_ofs = -ret;  

		 
		ret = send(conn->t.sock.fd, trash.str + ret + conn->send_proxy_ofs, -conn->send_proxy_ofs,
			   (conn->flags & CO_FL_DATA_WR_ENA) ? MSG_MORE : 0);

		if (ret == 0)
			goto out_wait;

		if (ret < 0) {
			if (errno == EAGAIN || errno == ENOTCONN)
				goto out_wait;
			if (errno == EINTR)
				continue;
			conn->flags |= CO_FL_SOCK_RD_SH | CO_FL_SOCK_WR_SH;
			goto out_error;
		}

		conn->send_proxy_ofs += ret;  
		if (conn->send_proxy_ofs != 0)
			goto out_wait;

		 
		break;
	}

	 
	if (conn->flags & CO_FL_WAIT_L4_CONN)
		conn->flags &= ~CO_FL_WAIT_L4_CONN;
	conn->flags &= ~flag;
	return 1;

 out_error:
	 
	conn->flags |= CO_FL_ERROR;
	return 0;

 out_wait:
	__conn_sock_stop_recv(conn);
	fd_cant_send(conn->t.sock.fd);
	return 0;
}
