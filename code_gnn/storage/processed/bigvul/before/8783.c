int udhcpc_main(int argc UNUSED_PARAM, char **argv)
{
	uint8_t *message;
	const char *str_V, *str_h, *str_F, *str_r;
	IF_FEATURE_UDHCPC_ARPING(const char *str_a = "2000";)
	IF_FEATURE_UDHCP_PORT(char *str_P;)
	void *clientid_mac_ptr;
	llist_t *list_O = NULL;
	llist_t *list_x = NULL;
	int tryagain_timeout = 20;
	int discover_timeout = 3;
	int discover_retries = 3;
	uint32_t server_addr = server_addr;  
	uint32_t requested_ip = 0;
	uint32_t xid = xid;  
	int packet_num;
	int timeout;  
	unsigned already_waited_sec;
	unsigned opt;
	IF_FEATURE_UDHCPC_ARPING(unsigned arpping_ms;)
	int retval;

	setup_common_bufsiz();

	 
	IF_FEATURE_UDHCP_PORT(SERVER_PORT = 67;)
	IF_FEATURE_UDHCP_PORT(CLIENT_PORT = 68;)
	client_config.interface = "eth0";
	client_config.script = CONFIG_UDHCPC_DEFAULT_SCRIPT;
	str_V = "udhcp "BB_VER;

	 
	opt = getopt32long(argv, "^"
		 
		"CV:H:h:F:i:np:qRr:s:T:+t:+SA:+O:*ox:*fB"
		USE_FOR_MMU("b")
		IF_FEATURE_UDHCPC_ARPING("a::")
		IF_FEATURE_UDHCP_PORT("P:")
		"v"
		"\0" IF_UDHCP_VERBOSE("vv")  
		, udhcpc_longopts
		, &str_V, &str_h, &str_h, &str_F
		, &client_config.interface, &client_config.pidfile  
		, &str_r  
		, &client_config.script  
		, &discover_timeout, &discover_retries, &tryagain_timeout  
		, &list_O
		, &list_x
		IF_FEATURE_UDHCPC_ARPING(, &str_a)
		IF_FEATURE_UDHCP_PORT(, &str_P)
		IF_UDHCP_VERBOSE(, &dhcp_verbose)
	);
	if (opt & (OPT_h|OPT_H)) {
		bb_error_msg("option -h NAME is deprecated, use -x hostname:NAME");
		client_config.hostname = alloc_dhcp_option(DHCP_HOST_NAME, str_h, 0);
	}
	if (opt & OPT_F) {
		 
		client_config.fqdn = alloc_dhcp_option(DHCP_FQDN, str_F, 3);
		 
		client_config.fqdn[OPT_DATA + 0] = 0x1;
		 
		 
	}
	if (opt & OPT_r)
		requested_ip = inet_addr(str_r);
#if ENABLE_FEATURE_UDHCP_PORT
	if (opt & OPT_P) {
		CLIENT_PORT = xatou16(str_P);
		SERVER_PORT = CLIENT_PORT - 1;
	}
#endif
	IF_FEATURE_UDHCPC_ARPING(arpping_ms = xatou(str_a);)
	while (list_O) {
		char *optstr = llist_pop(&list_O);
		unsigned n = bb_strtou(optstr, NULL, 0);
		if (errno || n > 254) {
			n = udhcp_option_idx(optstr, dhcp_option_strings);
			n = dhcp_optflags[n].code;
		}
		client_config.opt_mask[n >> 3] |= 1 << (n & 7);
	}
	if (!(opt & OPT_o)) {
		unsigned i, n;
		for (i = 0; (n = dhcp_optflags[i].code) != 0; i++) {
			if (dhcp_optflags[i].flags & OPTION_REQ) {
				client_config.opt_mask[n >> 3] |= 1 << (n & 7);
			}
		}
	}
	while (list_x) {
		char *optstr = xstrdup(llist_pop(&list_x));
		udhcp_str2optset(optstr, &client_config.options,
				dhcp_optflags, dhcp_option_strings,
				  0
		);
		free(optstr);
	}

	if (udhcp_read_interface(client_config.interface,
			&client_config.ifindex,
			NULL,
			client_config.client_mac)
	) {
		return 1;
	}

	clientid_mac_ptr = NULL;
	if (!(opt & OPT_C) && !udhcp_find_option(client_config.options, DHCP_CLIENT_ID)) {
		 
		client_config.clientid = alloc_dhcp_option(DHCP_CLIENT_ID, "", 7);
		client_config.clientid[OPT_DATA] = 1;  
		clientid_mac_ptr = client_config.clientid + OPT_DATA+1;
		memcpy(clientid_mac_ptr, client_config.client_mac, 6);
	}
	if (str_V[0] != '\0') {
		client_config.vendorclass = alloc_dhcp_option(DHCP_VENDOR, str_V, 0);
	}

#if !BB_MMU
	 
	if (!(opt & OPT_f)) {
		bb_daemonize_or_rexec(0  , argv);
		logmode = LOGMODE_NONE;
	}
#endif
	if (opt & OPT_S) {
		openlog(applet_name, LOG_PID, LOG_DAEMON);
		logmode |= LOGMODE_SYSLOG;
	}

	 
	bb_sanitize_stdio();
	 
	write_pidfile(client_config.pidfile);
	 
	bb_error_msg("started, v"BB_VER);
	 
	udhcp_sp_setup();
	 
	srand(monotonic_us());

	state = INIT_SELECTING;
	udhcp_run_script(NULL, "deconfig");
	change_listen_mode(LISTEN_RAW);
	packet_num = 0;
	timeout = 0;
	already_waited_sec = 0;

	 
	for (;;) {
		int tv;
		struct pollfd pfds[2];
		struct dhcp_packet packet;
		 
		unsigned timestamp_before_wait = timestamp_before_wait;


		 

		udhcp_sp_fd_set(pfds, sockfd);

		tv = timeout - already_waited_sec;
		retval = 0;
		 
		if (tv > 0) {
			log1("waiting %u seconds", tv);
			timestamp_before_wait = (unsigned)monotonic_sec();
			retval = poll(pfds, 2, tv < INT_MAX/1000 ? tv * 1000 : INT_MAX);
			if (retval < 0) {
				 
				if (errno == EINTR) {
					already_waited_sec += (unsigned)monotonic_sec() - timestamp_before_wait;
					continue;
				}
				 
				bb_perror_msg_and_die("poll");
			}
		}

		 
		if (retval == 0) {
			 
			if (udhcp_read_interface(client_config.interface,
					&client_config.ifindex,
					NULL,
					client_config.client_mac)
			) {
				goto ret0;  
			}
			if (clientid_mac_ptr)
				memcpy(clientid_mac_ptr, client_config.client_mac, 6);

			 
			already_waited_sec = 0;

			switch (state) {
			case INIT_SELECTING:
				if (!discover_retries || packet_num < discover_retries) {
					if (packet_num == 0)
						xid = random_xid();
					 
					send_discover(xid, requested_ip);
					timeout = discover_timeout;
					packet_num++;
					continue;
				}
 leasefail:
				udhcp_run_script(NULL, "leasefail");
#if BB_MMU  
				if (opt & OPT_b) {  
					bb_error_msg("no lease, forking to background");
					client_background();
					 
					opt = ((opt & ~OPT_b) | OPT_f);
				} else
#endif
				if (opt & OPT_n) {  
					bb_error_msg("no lease, failing");
					retval = 1;
					goto ret;
				}
				 
				timeout = tryagain_timeout;
				packet_num = 0;
				continue;
			case REQUESTING:
				if (packet_num < 3) {
					 
					send_select(xid, server_addr, requested_ip);
					timeout = discover_timeout;
					packet_num++;
					continue;
				}
				 
				change_listen_mode(LISTEN_RAW);
				state = INIT_SELECTING;
				goto leasefail;
			case BOUND:
				 
				state = RENEWING;
				client_config.first_secs = 0;  
				change_listen_mode(LISTEN_KERNEL);
				log1("entering renew state");
				 
			case RENEW_REQUESTED:  
			case_RENEW_REQUESTED:
			case RENEWING:
				if (timeout >= 60) {
					 
			 
					if (send_renew(xid, server_addr, requested_ip) >= 0) {
						timeout >>= 1;
						continue;
					}
					 
				}
				 
				log1("entering rebinding state");
				state = REBINDING;
				 
			case REBINDING:
				 
				change_listen_mode(LISTEN_RAW);
				 
				if (timeout > 0) {
					 
					send_renew(xid, 0  , requested_ip);
					timeout >>= 1;
					continue;
				}
				 
				bb_error_msg("lease lost, entering init state");
				udhcp_run_script(NULL, "deconfig");
				state = INIT_SELECTING;
				client_config.first_secs = 0;  
				 
				packet_num = 0;
				continue;
			 
			}
			 
			timeout = INT_MAX;
			continue;  
		}  

		 

		 
		switch (udhcp_sp_read()) {
		case SIGUSR1:
			client_config.first_secs = 0;  
			already_waited_sec = 0;
			perform_renew();
			if (state == RENEW_REQUESTED) {
				 
				if (timeout > 60)
					timeout = 60;
				goto case_RENEW_REQUESTED;
			}
			 
			packet_num = 0;
			 
			timeout = 0;
			continue;
		case SIGUSR2:
			perform_release(server_addr, requested_ip);
			timeout = INT_MAX;
			continue;
		case SIGTERM:
			bb_error_msg("received %s", "SIGTERM");
			goto ret0;
		}

		 
		if (!pfds[1].revents)
			continue;  

		{
			int len;

			 
			if (listen_mode == LISTEN_KERNEL)
				len = udhcp_recv_kernel_packet(&packet, sockfd);
			else
				len = udhcp_recv_raw_packet(&packet, sockfd);
			if (len == -1) {
				 
				bb_error_msg("read error: "STRERROR_FMT", reopening socket" STRERROR_ERRNO);
				sleep(discover_timeout);  
				change_listen_mode(listen_mode);  
			}
			 
			already_waited_sec += (unsigned)monotonic_sec() - timestamp_before_wait;
			if (len < 0)
				continue;
		}

		if (packet.xid != xid) {
			log1("xid %x (our is %x), ignoring packet",
				(unsigned)packet.xid, (unsigned)xid);
			continue;
		}

		 
		if (packet.hlen != 6
		 || memcmp(packet.chaddr, client_config.client_mac, 6) != 0
		) {
			log1("chaddr does not match, ignoring packet");  
			continue;
		}

		message = udhcp_get_option(&packet, DHCP_MESSAGE_TYPE);
		if (message == NULL) {
			bb_error_msg("no message type option, ignoring packet");
			continue;
		}

		switch (state) {
		case INIT_SELECTING:
			 
			if (*message == DHCPOFFER) {
				uint8_t *temp;

 
 
				server_addr = 0;
				temp = udhcp_get_option32(&packet, DHCP_SERVER_ID);
				if (!temp) {
					bb_error_msg("no server ID, using 0.0.0.0");
				} else {
					 
					move_from_unaligned32(server_addr, temp);
				}
				 
				requested_ip = packet.yiaddr;

				 
				state = REQUESTING;
				timeout = 0;
				packet_num = 0;
				already_waited_sec = 0;
			}
			continue;
		case REQUESTING:
		case RENEWING:
		case RENEW_REQUESTED:
		case REBINDING:
			if (*message == DHCPACK) {
				unsigned start;
				uint32_t lease_seconds;
				struct in_addr temp_addr;
				uint8_t *temp;

				temp = udhcp_get_option32(&packet, DHCP_LEASE_TIME);
				if (!temp) {
					bb_error_msg("no lease time with ACK, using 1 hour lease");
					lease_seconds = 60 * 60;
				} else {
					 
					move_from_unaligned32(lease_seconds, temp);
					lease_seconds = ntohl(lease_seconds);
					 
					 
					if (lease_seconds < 2 * 61)
						lease_seconds = 2 * 61;
				}
#if ENABLE_FEATURE_UDHCPC_ARPING
				if (opt & OPT_a) {
 
					if (!arpping(packet.yiaddr,
							NULL,
							(uint32_t) 0,
							client_config.client_mac,
							client_config.interface,
							arpping_ms)
					) {
						bb_error_msg("offered address is in use "
							"(got ARP reply), declining");
						send_decline(  server_addr, packet.yiaddr);

						if (state != REQUESTING)
							udhcp_run_script(NULL, "deconfig");
						change_listen_mode(LISTEN_RAW);
						state = INIT_SELECTING;
						client_config.first_secs = 0;  
						requested_ip = 0;
						timeout = tryagain_timeout;
						packet_num = 0;
						already_waited_sec = 0;
						continue;  
					}
				}
#endif
				 
				temp_addr.s_addr = packet.yiaddr;
				bb_error_msg("lease of %s obtained, lease time %u",
					inet_ntoa(temp_addr), (unsigned)lease_seconds);
				requested_ip = packet.yiaddr;

				start = monotonic_sec();
				udhcp_run_script(&packet, state == REQUESTING ? "bound" : "renew");
				already_waited_sec = (unsigned)monotonic_sec() - start;
				timeout = lease_seconds / 2;
				if ((unsigned)timeout < already_waited_sec) {
					 
					timeout = already_waited_sec = 0;
				}

				state = BOUND;
				change_listen_mode(LISTEN_NONE);
				if (opt & OPT_q) {  
					goto ret0;
				}
				 
				opt &= ~OPT_n;
#if BB_MMU  
				if (!(opt & OPT_f)) {
					client_background();
					 
					opt = ((opt & ~OPT_b) | OPT_f);
				}
#endif
				 
				 

				continue;  
			}
			if (*message == DHCPNAK) {
				 
				if (server_addr != 0) {
					uint32_t svid;
					uint8_t *temp;

					temp = udhcp_get_option32(&packet, DHCP_SERVER_ID);
					if (!temp) {
 non_matching_svid:
						log1("received DHCP NAK with wrong"
							" server ID, ignoring packet");
						continue;
					}
					move_from_unaligned32(svid, temp);
					if (svid != server_addr)
						goto non_matching_svid;
				}
				 
				bb_error_msg("received %s", "DHCP NAK");
				udhcp_run_script(&packet, "nak");
				if (state != REQUESTING)
					udhcp_run_script(NULL, "deconfig");
				change_listen_mode(LISTEN_RAW);
				sleep(3);  
				state = INIT_SELECTING;
				client_config.first_secs = 0;  
				requested_ip = 0;
				timeout = 0;
				packet_num = 0;
				already_waited_sec = 0;
			}
			continue;
		 
		 
		}
		 
	}  

 ret0:
	if (opt & OPT_R)  
		perform_release(server_addr, requested_ip);
	retval = 0;
 ret:
	 
		remove_pidfile(client_config.pidfile);
	return retval;
}
