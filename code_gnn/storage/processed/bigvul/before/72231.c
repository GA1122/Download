process_server_config_line(ServerOptions *options, char *line,
    const char *filename, int linenum, int *activep,
    struct connection_info *connectinfo)
{
	char *cp, **charptr, *arg, *p;
	int cmdline = 0, *intptr, value, value2, n, port;
	SyslogFacility *log_facility_ptr;
	LogLevel *log_level_ptr;
	ServerOpCodes opcode;
	u_int i, flags = 0;
	size_t len;
	long long val64;
	const struct multistate *multistate_ptr;

	cp = line;
	if ((arg = strdelim(&cp)) == NULL)
		return 0;
	 
	if (*arg == '\0')
		arg = strdelim(&cp);
	if (!arg || !*arg || *arg == '#')
		return 0;
	intptr = NULL;
	charptr = NULL;
	opcode = parse_token(arg, filename, linenum, &flags);

	if (activep == NULL) {  
		cmdline = 1;
		activep = &cmdline;
	}
	if (*activep && opcode != sMatch)
		debug3("%s:%d setting %s %s", filename, linenum, arg, cp);
	if (*activep == 0 && !(flags & SSHCFG_MATCH)) {
		if (connectinfo == NULL) {
			fatal("%s line %d: Directive '%s' is not allowed "
			    "within a Match block", filename, linenum, arg);
		} else {  
			while (arg)
				arg = strdelim(&cp);
			return 0;
		}
	}

	switch (opcode) {
	case sBadOption:
		return -1;
	case sPort:
		 
		if (options->ports_from_cmdline)
			return 0;
		if (options->num_ports >= MAX_PORTS)
			fatal("%s line %d: too many ports.",
			    filename, linenum);
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing port number.",
			    filename, linenum);
		options->ports[options->num_ports++] = a2port(arg);
		if (options->ports[options->num_ports-1] <= 0)
			fatal("%s line %d: Badly formatted port number.",
			    filename, linenum);
		break;

	case sLoginGraceTime:
		intptr = &options->login_grace_time;
 parse_time:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing time value.",
			    filename, linenum);
		if ((value = convtime(arg)) == -1)
			fatal("%s line %d: invalid time value.",
			    filename, linenum);
		if (*activep && *intptr == -1)
			*intptr = value;
		break;

	case sListenAddress:
		arg = strdelim(&cp);
		if (arg == NULL || *arg == '\0')
			fatal("%s line %d: missing address",
			    filename, linenum);
		 
		if (strchr(arg, '[') == NULL && (p = strchr(arg, ':')) != NULL
		    && strchr(p+1, ':') != NULL) {
			queue_listen_addr(options, arg, 0);
			break;
		}
		p = hpdelim(&arg);
		if (p == NULL)
			fatal("%s line %d: bad address:port usage",
			    filename, linenum);
		p = cleanhostname(p);
		if (arg == NULL)
			port = 0;
		else if ((port = a2port(arg)) <= 0)
			fatal("%s line %d: bad port number", filename, linenum);

		queue_listen_addr(options, p, port);

		break;

	case sAddressFamily:
		intptr = &options->address_family;
		multistate_ptr = multistate_addressfamily;
 parse_multistate:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing argument.",
			    filename, linenum);
		value = -1;
		for (i = 0; multistate_ptr[i].key != NULL; i++) {
			if (strcasecmp(arg, multistate_ptr[i].key) == 0) {
				value = multistate_ptr[i].value;
				break;
			}
		}
		if (value == -1)
			fatal("%s line %d: unsupported option \"%s\".",
			    filename, linenum, arg);
		if (*activep && *intptr == -1)
			*intptr = value;
		break;

	case sHostKeyFile:
		intptr = &options->num_host_key_files;
		if (*intptr >= MAX_HOSTKEYS)
			fatal("%s line %d: too many host keys specified (max %d).",
			    filename, linenum, MAX_HOSTKEYS);
		charptr = &options->host_key_files[*intptr];
 parse_filename:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing file name.",
			    filename, linenum);
		if (*activep && *charptr == NULL) {
			*charptr = derelativise_path(arg);
			 
			if (intptr != NULL)
				*intptr = *intptr + 1;
		}
		break;

	case sHostKeyAgent:
		charptr = &options->host_key_agent;
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing socket name.",
			    filename, linenum);
		if (*activep && *charptr == NULL)
			*charptr = !strcmp(arg, SSH_AUTHSOCKET_ENV_NAME) ?
			    xstrdup(arg) : derelativise_path(arg);
		break;

	case sHostCertificate:
		intptr = &options->num_host_cert_files;
		if (*intptr >= MAX_HOSTKEYS)
			fatal("%s line %d: too many host certificates "
			    "specified (max %d).", filename, linenum,
			    MAX_HOSTCERTS);
		charptr = &options->host_cert_files[*intptr];
		goto parse_filename;
		break;

	case sPidFile:
		charptr = &options->pid_file;
		goto parse_filename;

	case sPermitRootLogin:
		intptr = &options->permit_root_login;
		multistate_ptr = multistate_permitrootlogin;
		goto parse_multistate;

	case sIgnoreRhosts:
		intptr = &options->ignore_rhosts;
 parse_flag:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing yes/no argument.",
			    filename, linenum);
		value = 0;	 
		if (strcmp(arg, "yes") == 0)
			value = 1;
		else if (strcmp(arg, "no") == 0)
			value = 0;
		else
			fatal("%s line %d: Bad yes/no argument: %s",
				filename, linenum, arg);
		if (*activep && *intptr == -1)
			*intptr = value;
		break;

	case sIgnoreUserKnownHosts:
		intptr = &options->ignore_user_known_hosts;
		goto parse_flag;

	case sHostbasedAuthentication:
		intptr = &options->hostbased_authentication;
		goto parse_flag;

	case sHostbasedUsesNameFromPacketOnly:
		intptr = &options->hostbased_uses_name_from_packet_only;
		goto parse_flag;

	case sHostbasedAcceptedKeyTypes:
		charptr = &options->hostbased_key_types;
 parse_keytypes:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing argument.",
			    filename, linenum);
		if (!sshkey_names_valid2(*arg == '+' ? arg + 1 : arg, 1))
			fatal("%s line %d: Bad key types '%s'.",
			    filename, linenum, arg ? arg : "<NONE>");
		if (*activep && *charptr == NULL)
			*charptr = xstrdup(arg);
		break;

	case sHostKeyAlgorithms:
		charptr = &options->hostkeyalgorithms;
		goto parse_keytypes;

	case sPubkeyAuthentication:
		intptr = &options->pubkey_authentication;
		goto parse_flag;

	case sPubkeyAcceptedKeyTypes:
		charptr = &options->pubkey_key_types;
		goto parse_keytypes;

	case sKerberosAuthentication:
		intptr = &options->kerberos_authentication;
		goto parse_flag;

	case sKerberosOrLocalPasswd:
		intptr = &options->kerberos_or_local_passwd;
		goto parse_flag;

	case sKerberosTicketCleanup:
		intptr = &options->kerberos_ticket_cleanup;
		goto parse_flag;

	case sKerberosGetAFSToken:
		intptr = &options->kerberos_get_afs_token;
		goto parse_flag;

	case sGssAuthentication:
		intptr = &options->gss_authentication;
		goto parse_flag;

	case sGssCleanupCreds:
		intptr = &options->gss_cleanup_creds;
		goto parse_flag;

	case sGssStrictAcceptor:
		intptr = &options->gss_strict_acceptor;
		goto parse_flag;

	case sPasswordAuthentication:
		intptr = &options->password_authentication;
		goto parse_flag;

	case sKbdInteractiveAuthentication:
		intptr = &options->kbd_interactive_authentication;
		goto parse_flag;

	case sChallengeResponseAuthentication:
		intptr = &options->challenge_response_authentication;
		goto parse_flag;

	case sPrintMotd:
		intptr = &options->print_motd;
		goto parse_flag;

	case sPrintLastLog:
		intptr = &options->print_lastlog;
		goto parse_flag;

	case sX11Forwarding:
		intptr = &options->x11_forwarding;
		goto parse_flag;

	case sX11DisplayOffset:
		intptr = &options->x11_display_offset;
 parse_int:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing integer value.",
			    filename, linenum);
		value = atoi(arg);
		if (*activep && *intptr == -1)
			*intptr = value;
		break;

	case sX11UseLocalhost:
		intptr = &options->x11_use_localhost;
		goto parse_flag;

	case sXAuthLocation:
		charptr = &options->xauth_location;
		goto parse_filename;

	case sPermitTTY:
		intptr = &options->permit_tty;
		goto parse_flag;

	case sPermitUserRC:
		intptr = &options->permit_user_rc;
		goto parse_flag;

	case sStrictModes:
		intptr = &options->strict_modes;
		goto parse_flag;

	case sTCPKeepAlive:
		intptr = &options->tcp_keep_alive;
		goto parse_flag;

	case sEmptyPasswd:
		intptr = &options->permit_empty_passwd;
		goto parse_flag;

	case sPermitUserEnvironment:
		intptr = &options->permit_user_env;
		goto parse_flag;

	case sCompression:
		intptr = &options->compression;
		multistate_ptr = multistate_compression;
		goto parse_multistate;

	case sRekeyLimit:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%.200s line %d: Missing argument.", filename,
			    linenum);
		if (strcmp(arg, "default") == 0) {
			val64 = 0;
		} else {
			if (scan_scaled(arg, &val64) == -1)
				fatal("%.200s line %d: Bad number '%s': %s",
				    filename, linenum, arg, strerror(errno));
			if (val64 != 0 && val64 < 16)
				fatal("%.200s line %d: RekeyLimit too small",
				    filename, linenum);
		}
		if (*activep && options->rekey_limit == -1)
			options->rekey_limit = val64;
		if (cp != NULL) {  
			if (strcmp(cp, "none") == 0) {
				(void)strdelim(&cp);	 
				break;
			}
			intptr = &options->rekey_interval;
			goto parse_time;
		}
		break;

	case sGatewayPorts:
		intptr = &options->fwd_opts.gateway_ports;
		multistate_ptr = multistate_gatewayports;
		goto parse_multistate;

	case sUseDNS:
		intptr = &options->use_dns;
		goto parse_flag;

	case sLogFacility:
		log_facility_ptr = &options->log_facility;
		arg = strdelim(&cp);
		value = log_facility_number(arg);
		if (value == SYSLOG_FACILITY_NOT_SET)
			fatal("%.200s line %d: unsupported log facility '%s'",
			    filename, linenum, arg ? arg : "<NONE>");
		if (*log_facility_ptr == -1)
			*log_facility_ptr = (SyslogFacility) value;
		break;

	case sLogLevel:
		log_level_ptr = &options->log_level;
		arg = strdelim(&cp);
		value = log_level_number(arg);
		if (value == SYSLOG_LEVEL_NOT_SET)
			fatal("%.200s line %d: unsupported log level '%s'",
			    filename, linenum, arg ? arg : "<NONE>");
		if (*log_level_ptr == -1)
			*log_level_ptr = (LogLevel) value;
		break;

	case sAllowTcpForwarding:
		intptr = &options->allow_tcp_forwarding;
		multistate_ptr = multistate_tcpfwd;
		goto parse_multistate;

	case sAllowStreamLocalForwarding:
		intptr = &options->allow_streamlocal_forwarding;
		multistate_ptr = multistate_tcpfwd;
		goto parse_multistate;

	case sAllowAgentForwarding:
		intptr = &options->allow_agent_forwarding;
		goto parse_flag;

	case sUsePrivilegeSeparation:
		intptr = &use_privsep;
		multistate_ptr = multistate_privsep;
		goto parse_multistate;

	case sAllowUsers:
		while ((arg = strdelim(&cp)) && *arg != '\0') {
			if (options->num_allow_users >= MAX_ALLOW_USERS)
				fatal("%s line %d: too many allow users.",
				    filename, linenum);
			if (!*activep)
				continue;
			options->allow_users[options->num_allow_users++] =
			    xstrdup(arg);
		}
		break;

	case sDenyUsers:
		while ((arg = strdelim(&cp)) && *arg != '\0') {
			if (options->num_deny_users >= MAX_DENY_USERS)
				fatal("%s line %d: too many deny users.",
				    filename, linenum);
			if (!*activep)
				continue;
			options->deny_users[options->num_deny_users++] =
			    xstrdup(arg);
		}
		break;

	case sAllowGroups:
		while ((arg = strdelim(&cp)) && *arg != '\0') {
			if (options->num_allow_groups >= MAX_ALLOW_GROUPS)
				fatal("%s line %d: too many allow groups.",
				    filename, linenum);
			if (!*activep)
				continue;
			options->allow_groups[options->num_allow_groups++] =
			    xstrdup(arg);
		}
		break;

	case sDenyGroups:
		while ((arg = strdelim(&cp)) && *arg != '\0') {
			if (options->num_deny_groups >= MAX_DENY_GROUPS)
				fatal("%s line %d: too many deny groups.",
				    filename, linenum);
			if (!*activep)
				continue;
			options->deny_groups[options->num_deny_groups++] =
			    xstrdup(arg);
		}
		break;

	case sCiphers:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing argument.", filename, linenum);
		if (!ciphers_valid(*arg == '+' ? arg + 1 : arg))
			fatal("%s line %d: Bad SSH2 cipher spec '%s'.",
			    filename, linenum, arg ? arg : "<NONE>");
		if (options->ciphers == NULL)
			options->ciphers = xstrdup(arg);
		break;

	case sMacs:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing argument.", filename, linenum);
		if (!mac_valid(*arg == '+' ? arg + 1 : arg))
			fatal("%s line %d: Bad SSH2 mac spec '%s'.",
			    filename, linenum, arg ? arg : "<NONE>");
		if (options->macs == NULL)
			options->macs = xstrdup(arg);
		break;

	case sKexAlgorithms:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing argument.",
			    filename, linenum);
		if (!kex_names_valid(*arg == '+' ? arg + 1 : arg))
			fatal("%s line %d: Bad SSH2 KexAlgorithms '%s'.",
			    filename, linenum, arg ? arg : "<NONE>");
		if (options->kex_algorithms == NULL)
			options->kex_algorithms = xstrdup(arg);
		break;

	case sSubsystem:
		if (options->num_subsystems >= MAX_SUBSYSTEMS) {
			fatal("%s line %d: too many subsystems defined.",
			    filename, linenum);
		}
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing subsystem name.",
			    filename, linenum);
		if (!*activep) {
			arg = strdelim(&cp);
			break;
		}
		for (i = 0; i < options->num_subsystems; i++)
			if (strcmp(arg, options->subsystem_name[i]) == 0)
				fatal("%s line %d: Subsystem '%s' already defined.",
				    filename, linenum, arg);
		options->subsystem_name[options->num_subsystems] = xstrdup(arg);
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing subsystem command.",
			    filename, linenum);
		options->subsystem_command[options->num_subsystems] = xstrdup(arg);

		 
		p = xstrdup(arg);
		len = strlen(p) + 1;
		while ((arg = strdelim(&cp)) != NULL && *arg != '\0') {
			len += 1 + strlen(arg);
			p = xreallocarray(p, 1, len);
			strlcat(p, " ", len);
			strlcat(p, arg, len);
		}
		options->subsystem_args[options->num_subsystems] = p;
		options->num_subsystems++;
		break;

	case sMaxStartups:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing MaxStartups spec.",
			    filename, linenum);
		if ((n = sscanf(arg, "%d:%d:%d",
		    &options->max_startups_begin,
		    &options->max_startups_rate,
		    &options->max_startups)) == 3) {
			if (options->max_startups_begin >
			    options->max_startups ||
			    options->max_startups_rate > 100 ||
			    options->max_startups_rate < 1)
				fatal("%s line %d: Illegal MaxStartups spec.",
				    filename, linenum);
		} else if (n != 1)
			fatal("%s line %d: Illegal MaxStartups spec.",
			    filename, linenum);
		else
			options->max_startups = options->max_startups_begin;
		break;

	case sMaxAuthTries:
		intptr = &options->max_authtries;
		goto parse_int;

	case sMaxSessions:
		intptr = &options->max_sessions;
		goto parse_int;

	case sBanner:
		charptr = &options->banner;
		goto parse_filename;

	 
	case sAuthorizedKeysFile:
		if (*activep && options->num_authkeys_files == 0) {
			while ((arg = strdelim(&cp)) && *arg != '\0') {
				if (options->num_authkeys_files >=
				    MAX_AUTHKEYS_FILES)
					fatal("%s line %d: "
					    "too many authorized keys files.",
					    filename, linenum);
				options->authorized_keys_files[
				    options->num_authkeys_files++] =
				    tilde_expand_filename(arg, getuid());
			}
		}
		return 0;

	case sAuthorizedPrincipalsFile:
		charptr = &options->authorized_principals_file;
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing file name.",
			    filename, linenum);
		if (*activep && *charptr == NULL) {
			*charptr = tilde_expand_filename(arg, getuid());
			 
			if (intptr != NULL)
				*intptr = *intptr + 1;
		}
		break;

	case sClientAliveInterval:
		intptr = &options->client_alive_interval;
		goto parse_time;

	case sClientAliveCountMax:
		intptr = &options->client_alive_count_max;
		goto parse_int;

	case sAcceptEnv:
		while ((arg = strdelim(&cp)) && *arg != '\0') {
			if (strchr(arg, '=') != NULL)
				fatal("%s line %d: Invalid environment name.",
				    filename, linenum);
			if (options->num_accept_env >= MAX_ACCEPT_ENV)
				fatal("%s line %d: too many allow env.",
				    filename, linenum);
			if (!*activep)
				continue;
			options->accept_env[options->num_accept_env++] =
			    xstrdup(arg);
		}
		break;

	case sPermitTunnel:
		intptr = &options->permit_tun;
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: Missing yes/point-to-point/"
			    "ethernet/no argument.", filename, linenum);
		value = -1;
		for (i = 0; tunmode_desc[i].val != -1; i++)
			if (strcmp(tunmode_desc[i].text, arg) == 0) {
				value = tunmode_desc[i].val;
				break;
			}
		if (value == -1)
			fatal("%s line %d: Bad yes/point-to-point/ethernet/"
			    "no argument: %s", filename, linenum, arg);
		if (*activep && *intptr == -1)
			*intptr = value;
		break;

	case sMatch:
		if (cmdline)
			fatal("Match directive not supported as a command-line "
			   "option");
		value = match_cfg_line(&cp, linenum, connectinfo);
		if (value < 0)
			fatal("%s line %d: Bad Match condition", filename,
			    linenum);
		*activep = value;
		break;

	case sPermitOpen:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing PermitOpen specification",
			    filename, linenum);
		n = options->num_permitted_opens;	 
		if (strcmp(arg, "any") == 0) {
			if (*activep && n == -1) {
				channel_clear_adm_permitted_opens();
				options->num_permitted_opens = 0;
			}
			break;
		}
		if (strcmp(arg, "none") == 0) {
			if (*activep && n == -1) {
				options->num_permitted_opens = 1;
				channel_disable_adm_local_opens();
			}
			break;
		}
		if (*activep && n == -1)
			channel_clear_adm_permitted_opens();
		for (; arg != NULL && *arg != '\0'; arg = strdelim(&cp)) {
			p = hpdelim(&arg);
			if (p == NULL)
				fatal("%s line %d: missing host in PermitOpen",
				    filename, linenum);
			p = cleanhostname(p);
			if (arg == NULL || ((port = permitopen_port(arg)) < 0))
				fatal("%s line %d: bad port number in "
				    "PermitOpen", filename, linenum);
			if (*activep && n == -1)
				options->num_permitted_opens =
				    channel_add_adm_permitted_opens(p, port);
		}
		break;

	case sForceCommand:
		if (cp == NULL || *cp == '\0')
			fatal("%.200s line %d: Missing argument.", filename,
			    linenum);
		len = strspn(cp, WHITESPACE);
		if (*activep && options->adm_forced_command == NULL)
			options->adm_forced_command = xstrdup(cp + len);
		return 0;

	case sChrootDirectory:
		charptr = &options->chroot_directory;

		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing file name.",
			    filename, linenum);
		if (*activep && *charptr == NULL)
			*charptr = xstrdup(arg);
		break;

	case sTrustedUserCAKeys:
		charptr = &options->trusted_user_ca_keys;
		goto parse_filename;

	case sRevokedKeys:
		charptr = &options->revoked_keys_file;
		goto parse_filename;

	case sIPQoS:
		arg = strdelim(&cp);
		if ((value = parse_ipqos(arg)) == -1)
			fatal("%s line %d: Bad IPQoS value: %s",
			    filename, linenum, arg);
		arg = strdelim(&cp);
		if (arg == NULL)
			value2 = value;
		else if ((value2 = parse_ipqos(arg)) == -1)
			fatal("%s line %d: Bad IPQoS value: %s",
			    filename, linenum, arg);
		if (*activep) {
			options->ip_qos_interactive = value;
			options->ip_qos_bulk = value2;
		}
		break;

	case sVersionAddendum:
		if (cp == NULL || *cp == '\0')
			fatal("%.200s line %d: Missing argument.", filename,
			    linenum);
		len = strspn(cp, WHITESPACE);
		if (*activep && options->version_addendum == NULL) {
			if (strcasecmp(cp + len, "none") == 0)
				options->version_addendum = xstrdup("");
			else if (strchr(cp + len, '\r') != NULL)
				fatal("%.200s line %d: Invalid argument",
				    filename, linenum);
			else
				options->version_addendum = xstrdup(cp + len);
		}
		return 0;

	case sAuthorizedKeysCommand:
		if (cp == NULL)
			fatal("%.200s line %d: Missing argument.", filename,
			    linenum);
		len = strspn(cp, WHITESPACE);
		if (*activep && options->authorized_keys_command == NULL) {
			if (cp[len] != '/' && strcasecmp(cp + len, "none") != 0)
				fatal("%.200s line %d: AuthorizedKeysCommand "
				    "must be an absolute path",
				    filename, linenum);
			options->authorized_keys_command = xstrdup(cp + len);
		}
		return 0;

	case sAuthorizedKeysCommandUser:
		charptr = &options->authorized_keys_command_user;

		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing AuthorizedKeysCommandUser "
			    "argument.", filename, linenum);
		if (*activep && *charptr == NULL)
			*charptr = xstrdup(arg);
		break;

	case sAuthorizedPrincipalsCommand:
		if (cp == NULL)
			fatal("%.200s line %d: Missing argument.", filename,
			    linenum);
		len = strspn(cp, WHITESPACE);
		if (*activep &&
		    options->authorized_principals_command == NULL) {
			if (cp[len] != '/' && strcasecmp(cp + len, "none") != 0)
				fatal("%.200s line %d: "
				    "AuthorizedPrincipalsCommand must be "
				    "an absolute path", filename, linenum);
			options->authorized_principals_command =
			    xstrdup(cp + len);
		}
		return 0;

	case sAuthorizedPrincipalsCommandUser:
		charptr = &options->authorized_principals_command_user;

		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing "
			    "AuthorizedPrincipalsCommandUser argument.",
			    filename, linenum);
		if (*activep && *charptr == NULL)
			*charptr = xstrdup(arg);
		break;

	case sAuthenticationMethods:
		if (options->num_auth_methods == 0) {
			value = 0;  
			value2 = 0;  
			while ((arg = strdelim(&cp)) && *arg != '\0') {
				if (options->num_auth_methods >=
				    MAX_AUTH_METHODS)
					fatal("%s line %d: "
					    "too many authentication methods.",
					    filename, linenum);
				if (strcmp(arg, "any") == 0) {
					if (options->num_auth_methods > 0) {
						fatal("%s line %d: \"any\" "
						    "must appear alone in "
						    "AuthenticationMethods",
						    filename, linenum);
					}
					value = 1;
				} else if (value) {
					fatal("%s line %d: \"any\" must appear "
					    "alone in AuthenticationMethods",
					    filename, linenum);
				} else if (auth2_methods_valid(arg, 0) != 0) {
					fatal("%s line %d: invalid "
					    "authentication method list.",
					    filename, linenum);
				}
				value2 = 1;
				if (!*activep)
					continue;
				options->auth_methods[
				    options->num_auth_methods++] = xstrdup(arg);
			}
			if (value2 == 0) {
				fatal("%s line %d: no AuthenticationMethods "
				    "specified", filename, linenum);
			}
		}
		return 0;

	case sStreamLocalBindMask:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%s line %d: missing StreamLocalBindMask "
			    "argument.", filename, linenum);
		 
		value = strtol(arg, &p, 8);
		if (arg == p || value < 0 || value > 0777)
			fatal("%s line %d: Bad mask.", filename, linenum);
		if (*activep)
			options->fwd_opts.streamlocal_bind_mask = (mode_t)value;
		break;

	case sStreamLocalBindUnlink:
		intptr = &options->fwd_opts.streamlocal_bind_unlink;
		goto parse_flag;

	case sFingerprintHash:
		arg = strdelim(&cp);
		if (!arg || *arg == '\0')
			fatal("%.200s line %d: Missing argument.",
			    filename, linenum);
		if ((value = ssh_digest_alg_by_name(arg)) == -1)
			fatal("%.200s line %d: Invalid hash algorithm \"%s\".",
			    filename, linenum, arg);
		if (*activep)
			options->fingerprint_hash = value;
		break;

	case sDeprecated:
	case sIgnore:
	case sUnsupported:
		do_log2(opcode == sIgnore ?
		    SYSLOG_LEVEL_DEBUG2 : SYSLOG_LEVEL_INFO,
		    "%s line %d: %s option %s", filename, linenum,
		    opcode == sUnsupported ? "Unsupported" : "Deprecated", arg);
		while (arg)
		    arg = strdelim(&cp);
		break;

	default:
		fatal("%s line %d: Missing handler for opcode %s (%d)",
		    filename, linenum, arg, opcode);
	}
	if ((arg = strdelim(&cp)) != NULL && *arg != '\0')
		fatal("%s line %d: garbage at end of line; \"%.200s\".",
		    filename, linenum, arg);
	return 0;
}
