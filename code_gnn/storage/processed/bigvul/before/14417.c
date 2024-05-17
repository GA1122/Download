session_get_remote_name_or_ip(struct ssh *ssh, u_int utmp_size, int use_dns)
{
	const char *remote = "";

	if (utmp_size > 0)
		remote = auth_get_canonical_hostname(ssh, use_dns);
	if (utmp_size == 0 || strlen(remote) > utmp_size)
		remote = ssh_remote_ipaddr(ssh);
	return remote;
}
