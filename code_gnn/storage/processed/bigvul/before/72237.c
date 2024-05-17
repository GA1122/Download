authmethods_get(void)
{
	Authmethod *method = NULL;
	Buffer b;
	char *list;

	buffer_init(&b);
	for (method = authmethods; method->name != NULL; method++) {
		if (authmethod_is_enabled(method)) {
			if (buffer_len(&b) > 0)
				buffer_append(&b, ",", 1);
			buffer_append(&b, method->name, strlen(method->name));
		}
	}
	if ((list = sshbuf_dup_string(&b)) == NULL)
		fatal("%s: sshbuf_dup_string failed", __func__);
	buffer_free(&b);
	return list;
}
