aspath_unintern (struct aspath **aspath)
{
  struct aspath *ret;
  struct aspath *asp = *aspath;
  
  if (asp->refcnt)
    asp->refcnt--;

  if (asp->refcnt == 0)
    {
       
      ret = hash_release (ashash, asp);
      assert (ret != NULL);
      aspath_free (asp);
      *aspath = NULL;
    }
}
