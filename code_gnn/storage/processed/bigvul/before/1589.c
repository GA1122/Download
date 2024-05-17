aspath_intern (struct aspath *aspath)
{
  struct aspath *find;

   
  assert (aspath->refcnt == 0);
  assert (aspath->str);

   
  find = hash_get (ashash, aspath, hash_alloc_intern);
  if (find != aspath)
    aspath_free (aspath);

  find->refcnt++;

  return find;
}
