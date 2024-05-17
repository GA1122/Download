load_1(struct magic_set *ms, int action, const char *fn, int *errs,
   struct magic_entry_set *mset)
{
	char buffer[BUFSIZ + 1];
	char *line = NULL;
	size_t len;
	size_t lineno = 0;
	struct magic_entry me;

	php_stream *stream;

	TSRMLS_FETCH();

	ms->file = fn;
	stream = php_stream_open_wrapper((char *)fn, "rb", REPORT_ERRORS, NULL);

	if (stream == NULL) {
		if (errno != ENOENT)
			file_error(ms, errno, "cannot read magic file `%s'",
				   fn);
		(*errs)++;
		return;
	}

	memset(&me, 0, sizeof(me));
	 
	for (ms->line = 1; (line = php_stream_get_line(stream, buffer , BUFSIZ, &len)) != NULL; ms->line++) {
		if (len == 0)  
			continue;
		if (line[len - 1] == '\n') {
			lineno++;
			line[len - 1] = '\0';  
		}
		switch (line[0]) {
		case '\0':	 
		case '#':	 
			continue;
		case '!':
			if (line[1] == ':') {
				size_t i;

				for (i = 0; bang[i].name != NULL; i++) {
					if ((size_t)(len - 2) > bang[i].len &&
					    memcmp(bang[i].name, line + 2,
					    bang[i].len) == 0)
						break;
				}
				if (bang[i].name == NULL) {
					file_error(ms, 0,
					    "Unknown !: entry `%s'", line);
					(*errs)++;
					continue;
				}
				if (me.mp == NULL) {
					file_error(ms, 0,
					    "No current entry for :!%s type",
						bang[i].name);
					(*errs)++;
					continue;
				}
				if ((*bang[i].fun)(ms, &me,
				    line + bang[i].len + 2) != 0) {
					(*errs)++;
					continue;
				}
				continue;
			}
			 
		default:
		again:
			switch (parse(ms, &me, line, lineno, action)) {
			case 0:
				continue;
			case 1:
				(void)addentry(ms, &me, mset);
				goto again;
			default:
				(*errs)++;
				break;
			}
		}
	}
	if (me.mp)
		(void)addentry(ms, &me, mset);
	php_stream_close(stream);
}