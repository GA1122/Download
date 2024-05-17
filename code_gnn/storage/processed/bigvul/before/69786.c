node_get_platform(const node_t *node)
{
   
  if (node->ri)
    return node->ri->platform;
  else
    return NULL;
}
