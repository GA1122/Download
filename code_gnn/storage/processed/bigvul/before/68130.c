static oidc_provider_t *oidc_get_provider_for_issuer(request_rec *r,
		oidc_cfg *c, const char *issuer, apr_byte_t allow_discovery) {

	 
	oidc_provider_t *provider = NULL;
	if (oidc_provider_static_config(r, c, &provider) == FALSE)
		return NULL;

	 
	if (c->metadata_dir != NULL) {

		 
		if ((oidc_metadata_get(r, c, issuer, &provider, allow_discovery)
				== FALSE) || (provider == NULL)) {

			 
			oidc_error(r, "no provider metadata found for issuer \"%s\"",
					issuer);

			return NULL;
		}
	}

	return provider;
}
