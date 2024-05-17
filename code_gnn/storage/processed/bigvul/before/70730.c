evutil_getaddrinfo(const char *nodename, const char *servname,
    const struct evutil_addrinfo *hints_in, struct evutil_addrinfo **res)
{
#ifdef USE_NATIVE_GETADDRINFO
	struct evutil_addrinfo hints;
	int portnum=-1, need_np_hack, err;

	if (hints_in) {
		memcpy(&hints, hints_in, sizeof(hints));
	} else {
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
	}

#ifndef AI_ADDRCONFIG
	 
	if (hints.ai_family == PF_UNSPEC &&
	    (hints.ai_flags & EVUTIL_AI_ADDRCONFIG)) {
		evutil_adjust_hints_for_addrconfig_(&hints);
	}
#endif

#ifndef AI_NUMERICSERV
	 
	if (hints.ai_flags & EVUTIL_AI_NUMERICSERV) {
		if (servname && parse_numeric_servname(servname)<0)
			return EVUTIL_EAI_NONAME;
	}
#endif

	 
#ifdef _WIN32
	{
		int tmp_port;
		err = evutil_getaddrinfo_common_(nodename,servname,&hints,
		    res, &tmp_port);
		if (err == 0 ||
		    err == EVUTIL_EAI_MEMORY ||
		    err == EVUTIL_EAI_NONAME)
			return err;
		 
	}
#endif

	 
	need_np_hack = need_numeric_port_hack() && servname && !hints.ai_socktype
	    && ((portnum=parse_numeric_servname(servname)) >= 0);
	if (need_np_hack) {
		if (!nodename)
			return evutil_getaddrinfo_common_(
				NULL,servname,&hints, res, &portnum);
		servname = NULL;
	}

	if (need_socktype_protocol_hack()) {
		evutil_getaddrinfo_infer_protocols(&hints);
	}

	 
	EVUTIL_ASSERT((ALL_NONNATIVE_AI_FLAGS & ALL_NATIVE_AI_FLAGS) == 0);

	 
	hints.ai_flags &= ~ALL_NONNATIVE_AI_FLAGS;

	err = getaddrinfo(nodename, servname, &hints, res);
	if (need_np_hack)
		apply_numeric_port_hack(portnum, res);

	if (need_socktype_protocol_hack()) {
		if (apply_socktype_protocol_hack(*res) < 0) {
			evutil_freeaddrinfo(*res);
			*res = NULL;
			return EVUTIL_EAI_MEMORY;
		}
	}
	return err;
#else
	int port=0, err;
	struct hostent *ent = NULL;
	struct evutil_addrinfo hints;

	if (hints_in) {
		memcpy(&hints, hints_in, sizeof(hints));
	} else {
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
	}

	evutil_adjust_hints_for_addrconfig_(&hints);

	err = evutil_getaddrinfo_common_(nodename, servname, &hints, res, &port);
	if (err != EVUTIL_EAI_NEED_RESOLVE) {
		 
		return err;
	}

	err = 0;
	 
	{
#ifdef EVENT__HAVE_GETHOSTBYNAME_R_6_ARG
		 
		char buf[2048];
		struct hostent hostent;
		int r;
		r = gethostbyname_r(nodename, &hostent, buf, sizeof(buf), &ent,
		    &err);
#elif defined(EVENT__HAVE_GETHOSTBYNAME_R_5_ARG)
		char buf[2048];
		struct hostent hostent;
		ent = gethostbyname_r(nodename, &hostent, buf, sizeof(buf),
		    &err);
#elif defined(EVENT__HAVE_GETHOSTBYNAME_R_3_ARG)
		struct hostent_data data;
		struct hostent hostent;
		memset(&data, 0, sizeof(data));
		err = gethostbyname_r(nodename, &hostent, &data);
		ent = err ? NULL : &hostent;
#else
		 
		 
		ent = gethostbyname(nodename);
#ifdef _WIN32
		err = WSAGetLastError();
#else
		err = h_errno;
#endif
#endif

		 
		if (!ent) {
			 
			switch (err) {
			case TRY_AGAIN:
				return EVUTIL_EAI_AGAIN;
			case NO_RECOVERY:
			default:
				return EVUTIL_EAI_FAIL;
			case HOST_NOT_FOUND:
				return EVUTIL_EAI_NONAME;
			case NO_ADDRESS:
#if NO_DATA != NO_ADDRESS
			case NO_DATA:
#endif
				return EVUTIL_EAI_NODATA;
			}
		}

		if (ent->h_addrtype != hints.ai_family &&
		    hints.ai_family != PF_UNSPEC) {
			 
			return EVUTIL_EAI_NONAME;
		}

		 
		if (ent->h_length == 0)
			return EVUTIL_EAI_NODATA;

		 
		if (ent->h_addrtype != PF_INET && ent->h_addrtype != PF_INET6)
			return EVUTIL_EAI_FAMILY;

		*res = addrinfo_from_hostent(ent, port, &hints);
		if (! *res)
			return EVUTIL_EAI_MEMORY;
	}

	return 0;
#endif
}
