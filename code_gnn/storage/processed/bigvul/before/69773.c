node_exit_policy_is_exact(const node_t *node, sa_family_t family)
{
  if (family == AF_UNSPEC) {
    return 1;  
  } else if (family == AF_INET) {
    return node->ri != NULL;
  } else if (family == AF_INET6) {
    return 0;
  }
  tor_fragile_assert();
  return 1;
}