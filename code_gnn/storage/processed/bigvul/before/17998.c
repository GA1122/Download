ssh_remote_port(struct ssh *ssh)
{
	(void)ssh_remote_ipaddr(ssh);  
	return ssh->remote_port;
}
