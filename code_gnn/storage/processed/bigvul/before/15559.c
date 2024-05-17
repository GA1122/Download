check_agent_present(void)
{
	int r;

	if (options.forward_agent) {
		 
		if ((r = ssh_get_authentication_socket(NULL)) != 0) {
			options.forward_agent = 0;
			if (r != SSH_ERR_AGENT_NOT_PRESENT)
				debug("ssh_get_authentication_socket: %s",
				    ssh_err(r));
		}
	}
}
