set_unix_port(edge_connection_t *conn, rend_service_port_config_t *p)
{
  tor_assert(conn);
  tor_assert(p);
  tor_assert(p->is_unix_addr);

  conn->base_.socket_family = AF_UNIX;
  tor_addr_make_unspec(&conn->base_.addr);
  conn->base_.port = 1;
  conn->base_.address = tor_strdup(p->unix_addr);
  return 0;
}
