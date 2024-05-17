usage(void)
{
	fprintf(stderr, "%s, %s\n",
	    SSH_VERSION,
#ifdef WITH_OPENSSL
	    SSLeay_version(SSLEAY_VERSION)
#else
	    "without OpenSSL"
#endif
	);
	fprintf(stderr,
"usage: sshd [-46DdeiqTt] [-C connection_spec] [-c host_cert_file]\n"
"            [-E log_file] [-f config_file] [-g login_grace_time]\n"
"            [-h host_key_file] [-o option] [-p port] [-u len]\n"
	);
	exit(1);
}
