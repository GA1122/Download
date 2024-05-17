check_definition(const char *buf)
{
	const char *p;
	def_t* def;
	size_t def_name_len;
	char *str;

	if (buf[0] != '$')
		return false;

	if (!isalpha(buf[1]) && buf[1] != '_')
		return NULL;

	for (p = buf + 2; *p; p++) {
		if (*p == '=')
			break;
		if (!isalnum(*p) &&
		    !isdigit(*p) &&
		    *p != '_')
			return NULL;
	}

	def_name_len = (size_t)(p - &buf[1]);

	p += strspn(p, " \t");
	if (*p != '=')
		return NULL;

	if ((def = find_definition(&buf[1], def_name_len, true))) {
		FREE(def->value);
		def->fn = NULL;		 
	}
	else {
		def = MALLOC(sizeof(*def));
		def->name_len = def_name_len;
		str = MALLOC(def->name_len + 1);
		strncpy(str, &buf[1], def->name_len);
		str[def->name_len] = '\0';
		def->name = str;

		if (!LIST_EXISTS(defs))
			defs = alloc_list(free_definition, NULL);
		list_add(defs, def);
	}

	 
	p += strspn(p + 1, " \t") + 1;
	def->value_len = strlen(p);
	if (p[def->value_len - 1] == '\\') {
		 
		while (def->value_len >= 2 &&
		       isblank(p[def->value_len - 2]))
			def->value_len--;

		if (def->value_len < 2) {
			 
			p += def->value_len;
			def->value_len = 0;
		}
		def->multiline = true;
	} else
		def->multiline = false;

	str = MALLOC(def->value_len + 1);
	strcpy(str, p);
	def->value = str;

	 
	if (def->value_len >= 2 && def->multiline)
		def->value[def->value_len - 1] = DEF_LINE_END[0];

	return def;
}