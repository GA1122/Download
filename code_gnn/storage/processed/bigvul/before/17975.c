ssh_local_ipaddr(struct ssh *ssh)
{
	(void)ssh_remote_ipaddr(ssh);  
	return ssh->local_ipaddr;
}
