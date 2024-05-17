static gboolean npidentifier_cache_find_info(gpointer key, gpointer value, gpointer user_data)
{
  NPIdentifier *ident = (NPIdentifier)key;
  NPIdentifierInfo *npi = (NPIdentifierInfo *)value;
  NPIdentifierFindArgs *args = (NPIdentifierFindArgs *)user_data;
#if !defined(HAVE_G_HASH_TABLE_FIND) || defined(BUILD_GENERIC)
  if (args->ident)
	return FALSE;
#endif
  if (npi->string_len != args->info.string_len)
	return FALSE;
  if (args->info.string_len > 0) {		 
	if (memcmp(args->info.u.string, npi->u.string, args->info.string_len) == 0) {
	  args->ident = ident;
	  return TRUE;
	}
  }
  else {								 
	if (args->info.u.value == npi->u.value) {
	  args->ident = ident;
	  return TRUE;
	}
  }
  return FALSE;
}