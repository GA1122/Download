cached_NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers)
{
   
  invoke_NPN_GetStringIdentifiers(names, nameCount, identifiers);

#if USE_NPIDENTIFIER_CACHE
  if (use_npidentifier_cache()) {
	for (int i = 0; i < nameCount; i++) {
	  NPIdentifier ident = identifiers[i];
	  if (npidentifier_cache_lookup(ident) == NULL) {
		npidentifier_cache_reserve(1);
		npidentifier_cache_add_string(ident, names[i]);
	  }
	}
  }
#endif
}