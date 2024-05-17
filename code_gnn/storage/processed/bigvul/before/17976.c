ssh_local_port(struct ssh *ssh)
{
	(void)ssh_remote_ipaddr(ssh);  
	return ssh->local_port;
}
