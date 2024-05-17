fill_default_server_options(ServerOptions *options)
{
	int i;

	if (options->num_host_key_files == 0) {
		 
		options->host_key_files[options->num_host_key_files++] =
		    _PATH_HOST_RSA_KEY_FILE;
		options->host_key_files[options->num_host_key_files++] =
		    _PATH_HOST_DSA_KEY_FILE;
		options->host_key_files[options->num_host_key_files++] =
		    _PATH_HOST_ECDSA_KEY_FILE;
		options->host_key_files[options->num_host_key_files++] =
		    _PATH_HOST_ED25519_KEY_FILE;
	}
	 
	if (options->num_ports == 0)
		options->ports[options->num_ports++] = SSH_DEFAULT_PORT;
	if (options->address_family == -1)
		options->address_family = AF_UNSPEC;
	if (options->listen_addrs == NULL)
		add_listen_addr(options, NULL, 0);
	if (options->pid_file == NULL)
		options->pid_file = xstrdup(_PATH_SSH_DAEMON_PID_FILE);
	if (options->login_grace_time == -1)
		options->login_grace_time = 120;
	if (options->permit_root_login == PERMIT_NOT_SET)
		options->permit_root_login = PERMIT_NO_PASSWD;
	if (options->ignore_rhosts == -1)
		options->ignore_rhosts = 1;
	if (options->ignore_user_known_hosts == -1)
		options->ignore_user_known_hosts = 0;
	if (options->print_motd == -1)
		options->print_motd = 1;
	if (options->print_lastlog == -1)
		options->print_lastlog = 1;
	if (options->x11_forwarding == -1)
		options->x11_forwarding = 0;
	if (options->x11_display_offset == -1)
		options->x11_display_offset = 10;
	if (options->x11_use_localhost == -1)
		options->x11_use_localhost = 1;
	if (options->xauth_location == NULL)
		options->xauth_location = xstrdup(_PATH_XAUTH);
	if (options->permit_tty == -1)
		options->permit_tty = 1;
	if (options->permit_user_rc == -1)
		options->permit_user_rc = 1;
	if (options->strict_modes == -1)
		options->strict_modes = 1;
	if (options->tcp_keep_alive == -1)
		options->tcp_keep_alive = 1;
	if (options->log_facility == SYSLOG_FACILITY_NOT_SET)
		options->log_facility = SYSLOG_FACILITY_AUTH;
	if (options->log_level == SYSLOG_LEVEL_NOT_SET)
		options->log_level = SYSLOG_LEVEL_INFO;
	if (options->hostbased_authentication == -1)
		options->hostbased_authentication = 0;
	if (options->hostbased_uses_name_from_packet_only == -1)
		options->hostbased_uses_name_from_packet_only = 0;
	if (options->pubkey_authentication == -1)
		options->pubkey_authentication = 1;
	if (options->kerberos_authentication == -1)
		options->kerberos_authentication = 0;
	if (options->kerberos_or_local_passwd == -1)
		options->kerberos_or_local_passwd = 1;
	if (options->kerberos_ticket_cleanup == -1)
		options->kerberos_ticket_cleanup = 1;
	if (options->kerberos_get_afs_token == -1)
		options->kerberos_get_afs_token = 0;
	if (options->gss_authentication == -1)
		options->gss_authentication = 0;
	if (options->gss_cleanup_creds == -1)
		options->gss_cleanup_creds = 1;
	if (options->gss_strict_acceptor == -1)
		options->gss_strict_acceptor = 0;
	if (options->password_authentication == -1)
		options->password_authentication = 1;
	if (options->kbd_interactive_authentication == -1)
		options->kbd_interactive_authentication = 0;
	if (options->challenge_response_authentication == -1)
		options->challenge_response_authentication = 1;
	if (options->permit_empty_passwd == -1)
		options->permit_empty_passwd = 0;
	if (options->permit_user_env == -1)
		options->permit_user_env = 0;
	if (options->compression == -1)
		options->compression = COMP_DELAYED;
	if (options->rekey_limit == -1)
		options->rekey_limit = 0;
	if (options->rekey_interval == -1)
		options->rekey_interval = 0;
	if (options->allow_tcp_forwarding == -1)
		options->allow_tcp_forwarding = FORWARD_ALLOW;
	if (options->allow_streamlocal_forwarding == -1)
		options->allow_streamlocal_forwarding = FORWARD_ALLOW;
	if (options->allow_agent_forwarding == -1)
		options->allow_agent_forwarding = 1;
	if (options->fwd_opts.gateway_ports == -1)
		options->fwd_opts.gateway_ports = 0;
	if (options->max_startups == -1)
		options->max_startups = 100;
	if (options->max_startups_rate == -1)
		options->max_startups_rate = 30;		 
	if (options->max_startups_begin == -1)
		options->max_startups_begin = 10;
	if (options->max_authtries == -1)
		options->max_authtries = DEFAULT_AUTH_FAIL_MAX;
	if (options->max_sessions == -1)
		options->max_sessions = DEFAULT_SESSIONS_MAX;
	if (options->use_dns == -1)
		options->use_dns = 0;
	if (options->client_alive_interval == -1)
		options->client_alive_interval = 0;
	if (options->client_alive_count_max == -1)
		options->client_alive_count_max = 3;
	if (options->num_authkeys_files == 0) {
		options->authorized_keys_files[options->num_authkeys_files++] =
		    xstrdup(_PATH_SSH_USER_PERMITTED_KEYS);
		options->authorized_keys_files[options->num_authkeys_files++] =
		    xstrdup(_PATH_SSH_USER_PERMITTED_KEYS2);
	}
	if (options->permit_tun == -1)
		options->permit_tun = SSH_TUNMODE_NO;
	if (options->ip_qos_interactive == -1)
		options->ip_qos_interactive = IPTOS_LOWDELAY;
	if (options->ip_qos_bulk == -1)
		options->ip_qos_bulk = IPTOS_THROUGHPUT;
	if (options->version_addendum == NULL)
		options->version_addendum = xstrdup("");
	if (options->fwd_opts.streamlocal_bind_mask == (mode_t)-1)
		options->fwd_opts.streamlocal_bind_mask = 0177;
	if (options->fwd_opts.streamlocal_bind_unlink == -1)
		options->fwd_opts.streamlocal_bind_unlink = 0;
	if (options->fingerprint_hash == -1)
		options->fingerprint_hash = SSH_FP_HASH_DEFAULT;

	assemble_algorithms(options);

	 
	if (use_privsep == -1)
		use_privsep = PRIVSEP_ON;

#define CLEAR_ON_NONE(v) \
	do { \
		if (option_clear_or_none(v)) { \
			free(v); \
			v = NULL; \
		} \
	} while(0)
	CLEAR_ON_NONE(options->pid_file);
	CLEAR_ON_NONE(options->xauth_location);
	CLEAR_ON_NONE(options->banner);
	CLEAR_ON_NONE(options->trusted_user_ca_keys);
	CLEAR_ON_NONE(options->revoked_keys_file);
	CLEAR_ON_NONE(options->authorized_principals_file);
	CLEAR_ON_NONE(options->adm_forced_command);
	CLEAR_ON_NONE(options->chroot_directory);
	for (i = 0; i < options->num_host_key_files; i++)
		CLEAR_ON_NONE(options->host_key_files[i]);
	for (i = 0; i < options->num_host_cert_files; i++)
		CLEAR_ON_NONE(options->host_cert_files[i]);
#undef CLEAR_ON_NONE

	 
	if (options->num_auth_methods == 1 &&
	    strcmp(options->auth_methods[0], "any") == 0) {
		free(options->auth_methods[0]);
		options->auth_methods[0] = NULL;
		options->num_auth_methods = 0;
	}

}
