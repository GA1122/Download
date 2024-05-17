static bool add_mod_to_array_talloc(TALLOC_CTX *mem_ctx,
				    struct ldap_mod *mod,
				    struct ldap_mod **mods,
				    int *num_mods)
{
	*mods = talloc_realloc(mem_ctx, *mods, struct ldap_mod, (*num_mods)+1);

	if (*mods == NULL)
		return false;

	(*mods)[*num_mods] = *mod;
	*num_mods += 1;
	return true;
}
