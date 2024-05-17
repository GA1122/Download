node_ipv6_or_preferred(const node_t *node)
{
  const or_options_t *options = get_options();
  tor_addr_port_t ipv4_addr;
  node_assert_ok(node);

   
  if (!fascist_firewall_use_ipv6(options)) {
    return 0;
  } else if (node->ipv6_preferred || node_get_prim_orport(node, &ipv4_addr)) {
    return node_has_ipv6_orport(node);
  }
  return 0;
}
