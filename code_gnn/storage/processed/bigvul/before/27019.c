cached_NPN_IdentifierIsString(NPIdentifier ident)
{
#if USE_NPIDENTIFIER_CACHE
  if (use_npidentifier_cache()) {
	NPIdentifierInfo *npi = npidentifier_cache_lookup(ident);
	if (npi)
	  return npi->string_len > 0;
  }
#endif
   
  return invoke_NPN_IdentifierIsString(ident);
}