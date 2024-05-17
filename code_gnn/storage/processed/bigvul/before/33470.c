static int syslog_action_restricted(int type)
{
	if (dmesg_restrict)
		return 1;
	 
	return type != SYSLOG_ACTION_READ_ALL && type != SYSLOG_ACTION_SIZE_BUFFER;
}
