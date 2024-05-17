server_request_direct_tcpip(void)
{
	Channel *c = NULL;
	char *target, *originator;
	u_short target_port, originator_port;

	target = packet_get_string(NULL);
	target_port = packet_get_int();
	originator = packet_get_string(NULL);
	originator_port = packet_get_int();
	packet_check_eom();

	debug("server_request_direct_tcpip: originator %s port %d, target %s "
	    "port %d", originator, originator_port, target, target_port);

	 
	if ((options.allow_tcp_forwarding & FORWARD_LOCAL) != 0 &&
	    !no_port_forwarding_flag && !options.disable_forwarding) {
		c = channel_connect_to_port(target, target_port,
		    "direct-tcpip", "direct-tcpip");
	} else {
		logit("refused local port forward: "
		    "originator %s port %d, target %s port %d",
		    originator, originator_port, target, target_port);
	}

	free(originator);
	free(target);

	return c;
}