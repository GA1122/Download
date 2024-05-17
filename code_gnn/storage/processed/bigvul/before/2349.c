const char *ldb_dn_get_linearized(struct ldb_dn *dn)
{
	unsigned int i;
	size_t len;
	char *d, *n;

	if ( ! dn || ( dn->invalid)) return NULL;

	if (dn->linearized) return dn->linearized;

	if ( ! dn->components) {
		ldb_dn_mark_invalid(dn);
		return NULL;
	}

	if (dn->comp_num == 0) {
		dn->linearized = talloc_strdup(dn, "");
		if ( ! dn->linearized) return NULL;
		return dn->linearized;
	}

	 
	for (len = 0, i = 0; i < dn->comp_num; i++) {
		 
		len += strlen(dn->components[i].name);
		 
		len += (dn->components[i].value.length * 3);
		len += 2;  
	}
	dn->linearized = talloc_array(dn, char, len);
	if ( ! dn->linearized) return NULL;

	d = dn->linearized;

	for (i = 0; i < dn->comp_num; i++) {

		 
		n = dn->components[i].name;
		while (*n) *d++ = *n++;

		*d++ = '=';

		 
		d += ldb_dn_escape_internal( d,
				(char *)dn->components[i].value.data,
				dn->components[i].value.length);
		*d++ = ',';
	}

	*(--d) = '\0';

	 
	dn->linearized = talloc_realloc(dn, dn->linearized,
					char, (d - dn->linearized + 1));

	return dn->linearized;
}
