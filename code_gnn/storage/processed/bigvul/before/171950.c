static void btpan_cleanup_conn(btpan_conn_t* conn)
{
 if (conn)
 {
        conn->handle = -1;
        conn->state = -1;
        memset(&conn->peer, 0, sizeof(conn->peer));
        memset(&conn->eth_addr, 0, sizeof(conn->eth_addr));
        conn->local_role = conn->remote_role = 0;
 }
}
