verify_host_key_callback(Key *hostkey, struct ssh *ssh)
{
	if (verify_host_key(xxx_host, xxx_hostaddr, hostkey) == -1)
		fatal("Host key verification failed.");
	return 0;
}
