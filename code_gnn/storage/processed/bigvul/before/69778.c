node_get_all_orports(const node_t *node)
{
  smartlist_t *sl = smartlist_new();
  int valid = 0;

   
  if (node->ri != NULL) {
    SL_ADD_NEW_IPV4_AP(node->ri, or_port, sl, valid);
  }

   
  if (!valid && node->rs != NULL) {
    SL_ADD_NEW_IPV4_AP(node->rs, or_port, sl, valid);
  }

   
  valid = 0;
  if (node->ri != NULL) {
    SL_ADD_NEW_IPV6_AP(node->ri, ipv6_orport, sl, valid);
  }

  if (!valid && node->rs != NULL) {
    SL_ADD_NEW_IPV6_AP(node->rs, ipv6_orport, sl, valid);
  }

  if (!valid && node->md != NULL) {
    SL_ADD_NEW_IPV6_AP(node->md, ipv6_orport, sl, valid);
  }

  return sl;
}
