addenv(struct cgi_environment *env, const char *fmt, ...)
{
	size_t n, space;
	int truncated = 0;
	char *added;
	va_list ap;

	 
	space = (env->buflen - env->bufused);

	 
	n = strlen(fmt) + 2 + 128;

	do {
		if (space <= n) {
			 
			n = env->buflen + CGI_ENVIRONMENT_SIZE;
			added = (char *)mg_realloc_ctx(env->buf, n, env->conn->phys_ctx);
			if (!added) {
				 
				mg_cry_internal(
				    env->conn,
				    "%s: Cannot allocate memory for CGI variable [%s]",
				    __func__,
				    fmt);
				return;
			}
			env->buf = added;
			env->buflen = n;
			space = (env->buflen - env->bufused);
		}

		 
		added = env->buf + env->bufused;

		 
		va_start(ap, fmt);
		mg_vsnprintf(env->conn, &truncated, added, (size_t)space, fmt, ap);
		va_end(ap);

		 
		if (truncated) {
			 
			space = 0;
			n = 1;
		}
	} while (truncated);

	 
	n = strlen(added) + 1;
	env->bufused += n;

	 
	space = (env->varlen - env->varused);
	if (space < 2) {
		mg_cry_internal(env->conn,
		                "%s: Cannot register CGI variable [%s]",
		                __func__,
		                fmt);
		return;
	}

	 
	env->var[env->varused] = added;
	env->varused++;
}
