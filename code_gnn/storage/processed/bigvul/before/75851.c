smtpserver_handler(vector_t *strvec)
{
	int ret = -1;
	char *port_str = SMTP_PORT_STR;

	 
	if (vector_size(strvec) >= 3)
		port_str = strvec_slot(strvec,2);

	 
	if (!strpbrk(strvec_slot(strvec, 1), "-/"))
		ret = inet_stosockaddr(strvec_slot(strvec, 1), port_str, &global_data->smtp_server);

	if (ret < 0)
		domain_stosockaddr(strvec_slot(strvec, 1), port_str, &global_data->smtp_server);

	if (global_data->smtp_server.ss_family == AF_UNSPEC)
		report_config_error(CONFIG_GENERAL_ERROR, "Invalid smtp server %s %s", FMT_STR_VSLOT(strvec, 1), port_str);
}