static char *ldb_dn_canonical(TALLOC_CTX *mem_ctx, struct ldb_dn *dn, int ex_format) {
	unsigned int i;
	TALLOC_CTX *tmpctx;
	char *cracked = NULL;
	const char *format = (ex_format ? "\n" : "/" );

	if ( ! ldb_dn_validate(dn)) {
		return NULL;
	}

	tmpctx = talloc_new(mem_ctx);

	 
	for (i = dn->comp_num - 1; i != (unsigned int) -1; i--) {
		if (ldb_attr_cmp(dn->components[i].name, "dc") != 0) {
			break;
		}
		if (cracked) {
			cracked = talloc_asprintf(tmpctx, "%s.%s",
						  ldb_dn_escape_value(tmpctx,
							dn->components[i].value),
						  cracked);
		} else {
			cracked = ldb_dn_escape_value(tmpctx,
							dn->components[i].value);
		}
		if (!cracked) {
			goto done;
		}
	}

	 
	if (i == (unsigned int) -1) {
		cracked = talloc_strdup_append_buffer(cracked, format);
		talloc_steal(mem_ctx, cracked);
		goto done;
	}

	 
	for (; i > 0; i--) {
		cracked = talloc_asprintf_append_buffer(cracked, "/%s",
							ldb_dn_escape_value(tmpctx,
							dn->components[i].value));
		if (!cracked) {
			goto done;
		}
	}

	 
	cracked = talloc_asprintf_append_buffer(cracked, "%s%s", format,
						ldb_dn_escape_value(tmpctx,
							dn->components[i].value));

	talloc_steal(mem_ctx, cracked);
done:
	talloc_free(tmpctx);
	return cracked;
}
