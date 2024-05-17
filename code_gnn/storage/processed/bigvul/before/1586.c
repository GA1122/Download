aspath_hash_alloc (void *arg)
{
  const struct aspath *aspath = arg;
  struct aspath *new;

   
  assert (aspath->str);
  if (! aspath->str)
    return NULL;

   
  new = XMALLOC (MTYPE_AS_PATH, sizeof (struct aspath));

   
  new->refcnt = 0;
  new->segments = aspath->segments;
  new->str = aspath->str;
  new->str_len = aspath->str_len;

  return new;
}
