 int udhcpd_main(int argc UNUSED_PARAM, char **argv)
 {
 	int server_socket = -1, retval;
 	uint8_t *state;
 	unsigned timeout_end;
 	unsigned num_ips;
 	unsigned opt;
 	struct option_set *option;
 	char *str_I = str_I;
 	const char *str_a = "2000";
 	unsigned arpping_ms;
 	IF_FEATURE_UDHCP_PORT(char *str_P;)
 
 	setup_common_bufsiz();
 
 	IF_FEATURE_UDHCP_PORT(SERVER_PORT = 67;)
 	IF_FEATURE_UDHCP_PORT(CLIENT_PORT = 68;)
 
 	opt = getopt32(argv, "^"
 		"fSI:va:"IF_FEATURE_UDHCP_PORT("P:")
 		"\0"
 #if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 1
 		"vv"
 #endif
 		, &str_I
 		, &str_a
 		IF_FEATURE_UDHCP_PORT(, &str_P)
 		IF_UDHCP_VERBOSE(, &dhcp_verbose)
 		);
 	if (!(opt & 1)) {  
 		bb_daemonize_or_rexec(0, argv);
 		logmode = LOGMODE_NONE;
 	}
 	 
 	argv += optind;
 	if (opt & 2) {  
 		openlog(applet_name, LOG_PID, LOG_DAEMON);
 		logmode |= LOGMODE_SYSLOG;
 	}
 	if (opt & 4) {  
 		len_and_sockaddr *lsa = xhost_and_af2sockaddr(str_I, 0, AF_INET);
 		server_config.server_nip = lsa->u.sin.sin_addr.s_addr;
 		free(lsa);
 	}
 #if ENABLE_FEATURE_UDHCP_PORT
 	if (opt & 32) {  
 		SERVER_PORT = xatou16(str_P);
 		CLIENT_PORT = SERVER_PORT + 1;
 	}
 #endif
 	arpping_ms = xatou(str_a);
 
 	 
 	read_config(argv[0] ? argv[0] : DHCPD_CONF_FILE);
 	 
 	if (server_config.auto_time > INT_MAX / 1000)
 		server_config.auto_time = INT_MAX / 1000;
 
 	 
 	bb_sanitize_stdio();
 
 	 
 	write_pidfile(server_config.pidfile);
 	 
 
 	bb_error_msg("started, v"BB_VER);
 
 	option = udhcp_find_option(server_config.options, DHCP_LEASE_TIME);
 	server_config.max_lease_sec = DEFAULT_LEASE_TIME;
 	if (option) {
 		move_from_unaligned32(server_config.max_lease_sec, option->data + OPT_DATA);
 		server_config.max_lease_sec = ntohl(server_config.max_lease_sec);
 	}
 
 	 
 	num_ips = server_config.end_ip - server_config.start_ip + 1;
 	if (server_config.max_leases > num_ips) {
 		bb_error_msg("max_leases=%u is too big, setting to %u",
 			(unsigned)server_config.max_leases, num_ips);
 		server_config.max_leases = num_ips;
 	}
 
 	 
 	SET_PTR_TO_GLOBALS(xzalloc(server_config.max_leases * sizeof(g_leases[0])));
 
 	read_leases(server_config.lease_file);
 
 	if (udhcp_read_interface(server_config.interface,
 			&server_config.ifindex,
 			(server_config.server_nip == 0 ? &server_config.server_nip : NULL),
 			server_config.server_mac)
 	) {
 		retval = 1;
 		goto ret;
 	}
 
 	 
 	udhcp_sp_setup();
 
  continue_with_autotime:
 	timeout_end = monotonic_sec() + server_config.auto_time;
 	while (1) {  
 		struct pollfd pfds[2];
 		struct dhcp_packet packet;
 		int bytes;
 		int tv;
 		uint8_t *server_id_opt;
 		uint8_t *requested_ip_opt;
 		uint32_t requested_nip = requested_nip;  
 		uint32_t static_lease_nip;
 		struct dyn_lease *lease, fake_lease;
 
 		if (server_socket < 0) {
 			server_socket = udhcp_listen_socket(  SERVER_PORT,
 					server_config.interface);
 		}
 
 		udhcp_sp_fd_set(pfds, server_socket);
 
  new_tv:
 		tv = -1;
 		if (server_config.auto_time) {
 			tv = timeout_end - monotonic_sec();
 			if (tv <= 0) {
  write_leases:
 				write_leases();
 				goto continue_with_autotime;
 			}
 			tv *= 1000;
 		}
 
 		 
 		retval = poll(pfds, 2, tv);
 		if (retval <= 0) {
 			if (retval == 0)
 				goto write_leases;
 			if (errno == EINTR)
 				goto new_tv;
 			 
 			bb_perror_msg_and_die("poll");
 		}
 
 		if (pfds[0].revents) switch (udhcp_sp_read()) {
 		case SIGUSR1:
 			bb_error_msg("received %s", "SIGUSR1");
 			write_leases();
 			 
 			goto continue_with_autotime;
 		case SIGTERM:
 			bb_error_msg("received %s", "SIGTERM");
 			write_leases();
 			goto ret0;
 		}
 
 		 
 		if (!pfds[1].revents)
 			continue;  
 
 		 
 		bytes = udhcp_recv_kernel_packet(&packet, server_socket);
 		if (bytes < 0) {
 			 
 			if (bytes == -1 && errno != EINTR) {
 				log1("read error: "STRERROR_FMT", reopening socket" STRERROR_ERRNO);
 				close(server_socket);
 				server_socket = -1;
 			}
 			continue;
 		}
 		if (packet.hlen != 6) {
 			bb_error_msg("MAC length != 6, ignoring packet");
 			continue;
 		}
 		if (packet.op != BOOTREQUEST) {
 			bb_error_msg("not a REQUEST, ignoring packet");
 			continue;
 		}
 		state = udhcp_get_option(&packet, DHCP_MESSAGE_TYPE);
 		if (state == NULL || state[0] < DHCP_MINTYPE || state[0] > DHCP_MAXTYPE) {
 			bb_error_msg("no or bad message type option, ignoring packet");
 			continue;
  		}
  
  		 
		server_id_opt = udhcp_get_option(&packet, DHCP_SERVER_ID);
// 		server_id_opt = udhcp_get_option32(&packet, DHCP_SERVER_ID);
  		if (server_id_opt) {
  			uint32_t server_id_network_order;
  			move_from_unaligned32(server_id_network_order, server_id_opt);
 			if (server_id_network_order != server_config.server_nip) {
 				 
 				log1("server ID doesn't match, ignoring");
 				continue;
 			}
 		}
 
 		 
 		static_lease_nip = get_static_nip_by_mac(server_config.static_leases, &packet.chaddr);
 		if (static_lease_nip) {
 			bb_error_msg("found static lease: %x", static_lease_nip);
 			memcpy(&fake_lease.lease_mac, &packet.chaddr, 6);
 			fake_lease.lease_nip = static_lease_nip;
 			fake_lease.expires = 0;
 			lease = &fake_lease;
 		} else {
 			lease = find_lease_by_mac(packet.chaddr);
  		}
  
  		 
		requested_ip_opt = udhcp_get_option(&packet, DHCP_REQUESTED_IP);
// 		requested_ip_opt = udhcp_get_option32(&packet, DHCP_REQUESTED_IP);
  		if (requested_ip_opt) {
  			move_from_unaligned32(requested_nip, requested_ip_opt);
  		}
 
 		switch (state[0]) {
 
 		case DHCPDISCOVER:
 			log1("received %s", "DISCOVER");
 
 			send_offer(&packet, static_lease_nip, lease, requested_ip_opt, arpping_ms);
 			break;
 
 		case DHCPREQUEST:
 			log1("received %s", "REQUEST");
  
 			if (!requested_ip_opt) {
 				requested_nip = packet.ciaddr;
 				if (requested_nip == 0) {
 					log1("no requested IP and no ciaddr, ignoring");
 					break;
 				}
 			}
 			if (lease && requested_nip == lease->lease_nip) {
 				 
 				send_ACK(&packet, lease->lease_nip);
 				break;
 			}
 			 
 
 			if (server_id_opt     
 			 || requested_ip_opt  
 			) {
 				 
 				send_NAK(&packet);
 			}  
 
 			break;
 
 		case DHCPDECLINE:
 			 
 			log1("received %s", "DECLINE");
 			if (server_id_opt
 			 && requested_ip_opt
 			 && lease   
 			 && requested_nip == lease->lease_nip
 			) {
 				memset(lease->lease_mac, 0, sizeof(lease->lease_mac));
 				lease->expires = time(NULL) + server_config.decline_time;
 			}
 			break;
 
 		case DHCPRELEASE:
 			 
 			log1("received %s", "RELEASE");
 			if (server_id_opt
 			 && lease   
 			 && packet.ciaddr == lease->lease_nip
 			) {
 				lease->expires = time(NULL);
 			}
 			break;
 
 		case DHCPINFORM:
 			log1("received %s", "INFORM");
 			send_inform(&packet);
 			break;
 		}
 	}
  ret0:
 	retval = 0;
  ret:
 	 
 		remove_pidfile(server_config.pidfile);
 	return retval;
 }