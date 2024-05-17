node_supports_ed25519_link_authentication(const node_t *node)
{
   
  if (! node_get_ed25519_id(node))
    return 0;
  if (node->ri) {
    const char *protos = node->ri->protocol_list;
    if (protos == NULL)
      return 0;
    return protocol_list_supports_protocol(protos, PRT_LINKAUTH, 3);
  }
  if (node->rs) {
    return node->rs->supports_ed25519_link_handshake;
  }
  tor_assert_nonfatal_unreached_once();
  return 0;
}
