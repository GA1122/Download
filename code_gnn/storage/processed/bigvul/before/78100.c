static int asn1_decode(sc_context_t *ctx, struct sc_asn1_entry *asn1,
		       const u8 *in, size_t len, const u8 **newp, size_t *len_left,
		       int choice, int depth)
{
	int r, idx = 0;
	const u8 *p = in, *obj;
	struct sc_asn1_entry *entry = asn1;
	size_t left = len, objlen;

	sc_debug(ctx, SC_LOG_DEBUG_ASN1,
		 "%*.*scalled, left=%"SC_FORMAT_LEN_SIZE_T"u, depth %d%s\n",
		 depth, depth, "", left, depth, choice ? ", choice" : "");

	if (!p)
		return SC_ERROR_ASN1_OBJECT_NOT_FOUND;
	if (left < 2) {
		while (asn1->name && (asn1->flags & SC_ASN1_OPTIONAL))
			asn1++;
		 
		if (asn1->name == NULL)
			return 0;
		sc_debug(ctx, SC_LOG_DEBUG_ASN1, "End of ASN.1 stream, "
			      "non-optional field \"%s\" not found\n",
			      asn1->name);
		return SC_ERROR_ASN1_OBJECT_NOT_FOUND;
	}
	if (p[0] == 0 || p[0] == 0xFF || len == 0)
		return SC_ERROR_ASN1_END_OF_CONTENTS;

	for (idx = 0; asn1[idx].name != NULL; idx++) {
		entry = &asn1[idx];

		sc_debug(ctx, SC_LOG_DEBUG_ASN1, "Looking for '%s', tag 0x%x%s%s\n",
			entry->name, entry->tag, choice? ", CHOICE" : "",
			(entry->flags & SC_ASN1_OPTIONAL)? ", OPTIONAL": "");

		 
		if (entry->type == SC_ASN1_CHOICE) {
			r = asn1_decode(ctx,
				(struct sc_asn1_entry *) entry->parm,
				p, left, &p, &left, 1, depth + 1);
			if (r >= 0)
				r = 0;
			goto decode_ok;
		}

		obj = sc_asn1_skip_tag(ctx, &p, &left, entry->tag, &objlen);
		if (obj == NULL) {
			sc_debug(ctx, SC_LOG_DEBUG_ASN1, "'%s' not present\n", entry->name);
			if (choice)
				continue;
			if (entry->flags & SC_ASN1_OPTIONAL)
				continue;
			sc_debug(ctx, SC_LOG_DEBUG_ASN1, "mandatory ASN.1 object '%s' not found\n", entry->name);
			if (left) {
				u8 line[128], *linep = line;
				size_t i;

				line[0] = 0;
				for (i = 0; i < 10 && i < left; i++) {
					sprintf((char *) linep, "%02X ", p[i]);
					linep += 3;
				}
				sc_debug(ctx, SC_LOG_DEBUG_ASN1, "next tag: %s\n", line);
			}
			SC_FUNC_RETURN(ctx, SC_LOG_DEBUG_ASN1, SC_ERROR_ASN1_OBJECT_NOT_FOUND);
		}
		r = asn1_decode_entry(ctx, entry, obj, objlen, depth);

decode_ok:
		if (r)
			return r;
		if (choice)
			break;
 	}
 	if (choice && asn1[idx].name == NULL)  
		SC_FUNC_RETURN(ctx, SC_LOG_DEBUG_ASN1, SC_ERROR_ASN1_OBJECT_NOT_FOUND);
 	if (newp != NULL)
		*newp = p;
 	if (len_left != NULL)
		*len_left = left;
	if (choice)
		SC_FUNC_RETURN(ctx, SC_LOG_DEBUG_ASN1, idx);
	SC_FUNC_RETURN(ctx, SC_LOG_DEBUG_ASN1, 0);
}