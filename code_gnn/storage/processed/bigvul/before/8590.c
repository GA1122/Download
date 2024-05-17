static void ssh_sent(Plug plug, int bufsize)
{
    Ssh ssh = (Ssh) plug;
     
    if (bufsize < SSH_MAX_BACKLOG)
	ssh_throttle_all(ssh, 0, bufsize);
}
