const char *ldb_dn_get_casefold(struct ldb_dn *dn)
{
	unsigned int i;
	size_t len;
	char *d, *n;

	if (dn->casefold) return dn->casefold;

	if (dn->special) {
		dn->casefold = talloc_strdup(dn, dn->linearized);
		if (!dn->casefold) return NULL;
		dn->valid_case = true;
		return dn->casefold;
	}

	if ( ! ldb_dn_casefold_internal(dn)) {
		return NULL;
	}

	if (dn->comp_num == 0) {
		dn->casefold = talloc_strdup(dn, "");
		return dn->casefold;
	}

	 
	for (len = 0, i = 0; i < dn->comp_num; i++) {
		 
		len += strlen(dn->components[i].cf_name);
		 
		len += (dn->components[i].cf_value.length * 3);
		len += 2;  
	}
	dn->casefold = talloc_array(dn, char, len);
	if ( ! dn->casefold) return NULL;

	d = dn->casefold;

	for (i = 0; i < dn->comp_num; i++) {

		 
		n = dn->components[i].cf_name;
		while (*n) *d++ = *n++;

		*d++ = '=';

		 
		d += ldb_dn_escape_internal( d,
				(char *)dn->components[i].cf_value.data,
				dn->components[i].cf_value.length);
		*d++ = ',';
	}
	*(--d) = '\0';

	 
	dn->casefold = talloc_realloc(dn, dn->casefold,
				      char, strlen(dn->casefold) + 1);

	return dn->casefold;
}