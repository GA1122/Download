int blk_verify_command(unsigned char *cmd, fmode_t has_write_perm)
{
	struct blk_cmd_filter *filter = &blk_default_cmd_filter;

	 
	if (capable(CAP_SYS_RAWIO))
		return 0;

	 
	if (!filter)
		return -EPERM;

	 
	if (test_bit(cmd[0], filter->read_ok))
		return 0;

	 
	if (test_bit(cmd[0], filter->write_ok) && has_write_perm)
		return 0;

	return -EPERM;
}
