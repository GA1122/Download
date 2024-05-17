static void read_pathspec_from_stdin(struct rev_info *revs, struct strbuf *sb,
				     struct cmdline_pathspec *prune)
{
	while (strbuf_getwholeline(sb, stdin, '\n') != EOF) {
		int len = sb->len;
		if (len && sb->buf[len - 1] == '\n')
			sb->buf[--len] = '\0';
		ALLOC_GROW(prune->path, prune->nr + 1, prune->alloc);
		prune->path[prune->nr++] = xstrdup(sb->buf);
	}
}
