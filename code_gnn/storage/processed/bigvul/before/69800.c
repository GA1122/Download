node_has_ipv6_addr(const node_t *node)
{
   
  if (node->ri && tor_addr_is_valid(&node->ri->ipv6_addr, 0))
    return 1;
  if (node->rs && tor_addr_is_valid(&node->rs->ipv6_addr, 0))
    return 1;
  if (node->md && tor_addr_is_valid(&node->md->ipv6_addr, 0))
    return 1;

  return 0;
}
