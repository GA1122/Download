node_get_prim_orport(const node_t *node, tor_addr_port_t *ap_out)
{
  node_assert_ok(node);
  tor_assert(ap_out);

   

  RETURN_IPV4_AP(node->ri, or_port, ap_out);
  RETURN_IPV4_AP(node->rs, or_port, ap_out);
   

  return -1;
}
