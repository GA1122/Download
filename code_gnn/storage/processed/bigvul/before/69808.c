node_is_dir(const node_t *node)
{
  if (node->rs) {
    routerstatus_t * rs = node->rs;
     
    return rs->is_v2_dir;
  } else if (node->ri) {
    routerinfo_t * ri = node->ri;
     
    return ri->supports_tunnelled_dir_requests;
  } else {
    return 0;
  }
}
