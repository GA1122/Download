aspath_parse (struct stream *s, size_t length, int use32bit)
{
  struct aspath as;
  struct aspath *find;

   
   
  if (length % AS16_VALUE_SIZE )
    return NULL;

  memset (&as, 0, sizeof (struct aspath));
  if (assegments_parse (s, length, &as.segments, use32bit) < 0)
    return NULL;

   
  find = hash_get (ashash, &as, aspath_hash_alloc);

   
  assert (find);

   
  if (find->refcnt)
    {
      assegment_free_all (as.segments);
       
      XFREE (MTYPE_AS_STR, as.str);
    }

  find->refcnt++;

  return find;
}
