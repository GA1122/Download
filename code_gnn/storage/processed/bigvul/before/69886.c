connection_ap_can_use_exit(const entry_connection_t *conn,
                           const node_t *exit_node)
{
  const or_options_t *options = get_options();

  tor_assert(conn);
  tor_assert(conn->socks_request);
  tor_assert(exit_node);

   
  if (conn->chosen_exit_name) {
    const node_t *chosen_exit =
      node_get_by_nickname(conn->chosen_exit_name, 1);
    if (!chosen_exit || tor_memneq(chosen_exit->identity,
                               exit_node->identity, DIGEST_LEN)) {
       
      return 0;
    }
  }

  if (conn->use_begindir) {
     
    return 1;
  }

  if (conn->socks_request->command == SOCKS_COMMAND_CONNECT) {
    tor_addr_t addr, *addrp = NULL;
    addr_policy_result_t r;
    if (0 == tor_addr_parse(&addr, conn->socks_request->address)) {
      addrp = &addr;
    } else if (!conn->entry_cfg.ipv4_traffic && conn->entry_cfg.ipv6_traffic) {
      tor_addr_make_null(&addr, AF_INET6);
      addrp = &addr;
    } else if (conn->entry_cfg.ipv4_traffic && !conn->entry_cfg.ipv6_traffic) {
      tor_addr_make_null(&addr, AF_INET);
      addrp = &addr;
    }
    r = compare_tor_addr_to_node_policy(addrp, conn->socks_request->port,
                                        exit_node);
    if (r == ADDR_POLICY_REJECTED)
      return 0;  
    if (r == ADDR_POLICY_PROBABLY_REJECTED && !conn->chosen_exit_name)
      return 0;  
  } else if (SOCKS_COMMAND_IS_RESOLVE(conn->socks_request->command)) {
     
    if (!conn->chosen_exit_name && node_exit_policy_rejects_all(exit_node))
      return 0;
  }
  if (routerset_contains_node(options->ExcludeExitNodesUnion_, exit_node)) {
     
    return 0;
  }

  return 1;
}
