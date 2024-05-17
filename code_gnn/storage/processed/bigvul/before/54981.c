static int commit_match(struct commit *commit, struct rev_info *opt)
{
	int retval;
	const char *encoding;
	const char *message;
	struct strbuf buf = STRBUF_INIT;

	if (!opt->grep_filter.pattern_list && !opt->grep_filter.header_list)
		return 1;

	 
	if (opt->grep_filter.use_reflog_filter) {
		strbuf_addstr(&buf, "reflog ");
		get_reflog_message(&buf, opt->reflog_info);
		strbuf_addch(&buf, '\n');
	}

	 
	encoding = get_log_output_encoding();
	message = logmsg_reencode(commit, NULL, encoding);

	 
	if (buf.len)
		strbuf_addstr(&buf, message);

	if (opt->grep_filter.header_list && opt->mailmap) {
		if (!buf.len)
			strbuf_addstr(&buf, message);

		commit_rewrite_person(&buf, "\nauthor ", opt->mailmap);
		commit_rewrite_person(&buf, "\ncommitter ", opt->mailmap);
	}

	 
	if (opt->show_notes) {
		if (!buf.len)
			strbuf_addstr(&buf, message);
		format_display_notes(commit->object.oid.hash, &buf, encoding, 1);
	}

	 
	if (buf.len)
		retval = grep_buffer(&opt->grep_filter, buf.buf, buf.len);
	else
		retval = grep_buffer(&opt->grep_filter,
				     (char *)message, strlen(message));
	strbuf_release(&buf);
	unuse_commit_buffer(commit, message);
	return opt->invert_grep ? !retval : retval;
}
