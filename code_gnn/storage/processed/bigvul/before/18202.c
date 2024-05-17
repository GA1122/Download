void server_connect_own_ip_save(SERVER_CONNECT_REC *conn,
				IPADDR *ip4, IPADDR *ip6)
{
	if (ip4 == NULL || ip4->family == 0)
		g_free_and_null(conn->own_ip4);
	if (ip6 == NULL || ip6->family == 0)
		g_free_and_null(conn->own_ip6);

	if (ip4 != NULL && ip4->family != 0) {
		 
		if (conn->own_ip4 == NULL)
			conn->own_ip4 = g_new0(IPADDR, 1);
		memcpy(conn->own_ip4, ip4, sizeof(IPADDR));
	}

	if (ip6 != NULL && ip6->family != 0) {
		 
		if (conn->own_ip6 == NULL)
			conn->own_ip6 = g_new0(IPADDR, 1);
		memcpy(conn->own_ip6, ip6, sizeof(IPADDR));
	}
}