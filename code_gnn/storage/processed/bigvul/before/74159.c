create_peer_node(
	int		hmode,
	address_node *	addr,
	attr_val_fifo *	options
	)
{
	peer_node *my_node;
	attr_val *option;
	int freenode;
	int errflag = 0;

	my_node = emalloc_zero(sizeof(*my_node));

	 
	my_node->peerversion = NTP_VERSION;

	 
	my_node->host_mode = hmode;
	my_node->addr = addr;

	 

	while (options != NULL) {
		UNLINK_FIFO(option, *options, link);
		if (NULL == option) {
			free(options);
			break;
		}

		freenode = 1;
		 
		switch (option->attr) {

		case T_Flag:
			APPEND_G_FIFO(my_node->peerflags, option);
			freenode = 0;
			break;

		case T_Minpoll:
			if (option->value.i < NTP_MINPOLL ||
			    option->value.i > UCHAR_MAX) {
				msyslog(LOG_INFO,
					"minpoll: provided value (%d) is out of range [%d-%d])",
					option->value.i, NTP_MINPOLL,
					UCHAR_MAX);
				my_node->minpoll = NTP_MINPOLL;
			} else {
				my_node->minpoll = 
					(u_char)option->value.u;
			}
			break;

		case T_Maxpoll:
			if (option->value.i < 0 ||
			    option->value.i > NTP_MAXPOLL) {
				msyslog(LOG_INFO,
					"maxpoll: provided value (%d) is out of range [0-%d])",
					option->value.i, NTP_MAXPOLL);
				my_node->maxpoll = NTP_MAXPOLL;
			} else {
				my_node->maxpoll = 
					(u_char)option->value.u;
			}
			break;

		case T_Ttl:
			if (option->value.u >= MAX_TTL) {
				msyslog(LOG_ERR, "ttl: invalid argument");
				errflag = 1;
			} else {
				my_node->ttl = (u_char)option->value.u;
			}
			break;

		case T_Mode:
			if (option->value.u >= UCHAR_MAX) {
				msyslog(LOG_ERR, "mode: invalid argument");
				errflag = 1;
			} else {
				my_node->ttl = (u_char)option->value.u;
			}
			break;

		case T_Key:
			if (option->value.u >= KEYID_T_MAX) {
				msyslog(LOG_ERR, "key: invalid argument");
				errflag = 1;
			} else {
				my_node->peerkey = 
					(keyid_t)option->value.u;
			}
			break;

		case T_Version:
			if (option->value.u >= UCHAR_MAX) {
				msyslog(LOG_ERR, "version: invalid argument");
				errflag = 1;
			} else {
				my_node->peerversion = 
					(u_char)option->value.u;
			}
			break;

		case T_Ident:
			my_node->group = option->value.s;
			break;

		default:
			msyslog(LOG_ERR, 
				"Unknown peer/server option token %s",
				token_name(option->attr));
			errflag = 1;
		}
		if (freenode)
			free(option);
	}

	 
	if (errflag) {
		free(my_node);
		my_node = NULL;
	}

	return my_node;
}
