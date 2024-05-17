node_get_prim_addr_ipv4h(const node_t *node)
{
   
  if (node->ri && tor_addr_is_valid_ipv4h(node->ri->addr, 0)) {
    return node->ri->addr;
  } else if (node->rs && tor_addr_is_valid_ipv4h(node->rs->addr, 0)) {
    return node->rs->addr;
  }
  return 0;
}
