static void perform_release(uint32_t server_addr, uint32_t requested_ip)
{
	char buffer[sizeof("255.255.255.255")];
	struct in_addr temp_addr;

	 
	if (state == BOUND
	 || state == RENEWING
	 || state == REBINDING
	 || state == RENEW_REQUESTED
	) {
		temp_addr.s_addr = server_addr;
		strcpy(buffer, inet_ntoa(temp_addr));
		temp_addr.s_addr = requested_ip;
		bb_error_msg("unicasting a release of %s to %s",
				inet_ntoa(temp_addr), buffer);
		send_release(server_addr, requested_ip);  
	}
	bb_error_msg("entering released state");
 
	udhcp_run_script(NULL, "deconfig");

	change_listen_mode(LISTEN_NONE);
	state = RELEASED;
}
