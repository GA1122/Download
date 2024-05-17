sshd_exchange_identification(struct ssh *ssh, int sock_in, int sock_out)
{
	u_int i;
	int remote_major, remote_minor;
	char *s, *newline = "\n";
	char buf[256];			 
	char remote_version[256];	 

	xasprintf(&server_version_string, "SSH-%d.%d-%.100s%s%s%s",
	    PROTOCOL_MAJOR_2, PROTOCOL_MINOR_2, SSH_VERSION,
	    *options.version_addendum == '\0' ? "" : " ",
	    options.version_addendum, newline);

	 
	if (atomicio(vwrite, sock_out, server_version_string,
	    strlen(server_version_string))
	    != strlen(server_version_string)) {
		logit("Could not write ident string to %s port %d",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
		cleanup_exit(255);
	}

	 
	memset(buf, 0, sizeof(buf));
	for (i = 0; i < sizeof(buf) - 1; i++) {
		if (atomicio(read, sock_in, &buf[i], 1) != 1) {
			logit("Did not receive identification string "
			    "from %s port %d",
			    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
			cleanup_exit(255);
		}
		if (buf[i] == '\r') {
			buf[i] = 0;
			 
			if (i == 12 &&
			    strncmp(buf, "SSH-1.5-W1.0", 12) == 0)
				break;
			continue;
		}
		if (buf[i] == '\n') {
			buf[i] = 0;
			break;
		}
	}
	buf[sizeof(buf) - 1] = 0;
	client_version_string = xstrdup(buf);

	 
	if (sscanf(client_version_string, "SSH-%d.%d-%[^\n]\n",
	    &remote_major, &remote_minor, remote_version) != 3) {
		s = "Protocol mismatch.\n";
		(void) atomicio(vwrite, sock_out, s, strlen(s));
		logit("Bad protocol version identification '%.100s' "
		    "from %s port %d", client_version_string,
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
		close(sock_in);
		close(sock_out);
		cleanup_exit(255);
	}
	debug("Client protocol version %d.%d; client software version %.100s",
	    remote_major, remote_minor, remote_version);

	ssh->compat = compat_datafellows(remote_version);

	if ((ssh->compat & SSH_BUG_PROBE) != 0) {
		logit("probed from %s port %d with %s.  Don't panic.",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh),
		    client_version_string);
		cleanup_exit(255);
	}
	if ((ssh->compat & SSH_BUG_SCANNER) != 0) {
		logit("scanned from %s port %d with %s.  Don't panic.",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh),
		    client_version_string);
		cleanup_exit(255);
	}
	if ((ssh->compat & SSH_BUG_RSASIGMD5) != 0) {
		logit("Client version \"%.100s\" uses unsafe RSA signature "
		    "scheme; disabling use of RSA keys", remote_version);
	}
	if ((ssh->compat & SSH_BUG_DERIVEKEY) != 0) {
		fatal("Client version \"%.100s\" uses unsafe key agreement; "
		    "refusing connection", remote_version);
	}

	chop(server_version_string);
	debug("Local version string %.200s", server_version_string);

	if (remote_major == 2 ||
	    (remote_major == 1 && remote_minor == 99)) {
		enable_compat20();
	} else {
		s = "Protocol major versions differ.\n";
		(void) atomicio(vwrite, sock_out, s, strlen(s));
		close(sock_in);
		close(sock_out);
		logit("Protocol major versions differ for %s port %d: "
		    "%.200s vs. %.200s",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh),
		    server_version_string, client_version_string);
		cleanup_exit(255);
	}
}
