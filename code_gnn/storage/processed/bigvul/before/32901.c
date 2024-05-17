void __ext3_std_error (struct super_block * sb, const char * function,
		       int errno)
{
	char nbuf[16];
	const char *errstr;

	 
	if (errno == -EROFS && journal_current_handle() == NULL &&
	    (sb->s_flags & MS_RDONLY))
		return;

	errstr = ext3_decode_error(sb, errno, nbuf);
	ext3_msg(sb, KERN_CRIT, "error in %s: %s", function, errstr);

	ext3_handle_error(sb);
}
