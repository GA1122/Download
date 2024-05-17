static NOINLINE void send_offer(struct dhcp_packet *oldpacket,
		uint32_t static_lease_nip,
		struct dyn_lease *lease,
		uint8_t *requested_ip_opt,
		unsigned arpping_ms)
{
	struct dhcp_packet packet;
	uint32_t lease_time_sec;
	struct in_addr addr;

	init_packet(&packet, oldpacket, DHCPOFFER);

	 
	packet.yiaddr = static_lease_nip;
	 
	if (!static_lease_nip) {
		 
		uint32_t req_nip;
		const char *p_host_name;

		if (lease) {
			 
			packet.yiaddr = lease->lease_nip;
		}
		 
		else if (requested_ip_opt != NULL
		  
		 && (move_from_unaligned32(req_nip, requested_ip_opt), 1)
		  
		 && ntohl(req_nip) >= server_config.start_ip
		 && ntohl(req_nip) <= server_config.end_ip
		  
		 && (  !(lease = find_lease_by_nip(req_nip))  
		    || is_expired_lease(lease)  
		    )
		) {
			packet.yiaddr = req_nip;
		}
		else {
			 
			packet.yiaddr = find_free_or_expired_nip(oldpacket->chaddr, arpping_ms);
		}

		if (!packet.yiaddr) {
			bb_error_msg("no free IP addresses. OFFER abandoned");
			return;
		}
		 
		p_host_name = (const char*) udhcp_get_option(oldpacket, DHCP_HOST_NAME);
		lease = add_lease(packet.chaddr, packet.yiaddr,
				server_config.offer_time,
				p_host_name,
				p_host_name ? (unsigned char)p_host_name[OPT_LEN - OPT_DATA] : 0
		);
		if (!lease) {
			bb_error_msg("no free IP addresses. OFFER abandoned");
			return;
		}
	}

	lease_time_sec = select_lease_time(oldpacket);
	udhcp_add_simple_option(&packet, DHCP_LEASE_TIME, htonl(lease_time_sec));
	add_server_options(&packet);

	addr.s_addr = packet.yiaddr;
	bb_error_msg("sending OFFER of %s", inet_ntoa(addr));
	 
	send_packet(&packet,   0);
}
