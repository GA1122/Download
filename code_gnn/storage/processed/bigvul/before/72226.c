parse_server_config(ServerOptions *options, const char *filename, Buffer *conf,
    struct connection_info *connectinfo)
{
	int active, linenum, bad_options = 0;
	char *cp, *obuf, *cbuf;

	debug2("%s: config %s len %d", __func__, filename, buffer_len(conf));

	if ((obuf = cbuf = sshbuf_dup_string(conf)) == NULL)
		fatal("%s: sshbuf_dup_string failed", __func__);
	active = connectinfo ? 0 : 1;
	linenum = 1;
	while ((cp = strsep(&cbuf, "\n")) != NULL) {
		if (process_server_config_line(options, cp, filename,
		    linenum++, &active, connectinfo) != 0)
			bad_options++;
	}
	free(obuf);
	if (bad_options > 0)
		fatal("%s: terminating, %d bad configuration options",
		    filename, bad_options);
	process_queued_listen_addrs(options);
}
