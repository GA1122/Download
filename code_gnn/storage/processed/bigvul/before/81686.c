get_uri_type(const char *uri)
{
	int i;
	const char *hostend, *portbegin;
	char *portend;
	unsigned long port;

	 
	if ((uri[0] == '*') && (uri[1] == '\0')) {
		 
		return 1;
	}

	 
	for (i = 0; uri[i] != 0; i++) {
		if (uri[i] < 33) {
			 
			return 0;
		}
		if (uri[i] > 126) {
			 
			return 0;
		} else {
			switch (uri[i]) {
			case '"':   
			case '<':   
			case '>':   
			case '\\':  
			case '^':   
			case '`':   
			case '{':   
			case '|':   
			case '}':   
				return 0;
			default:
				 
				break;
			}
		}
	}

	 
	if (uri[0] == '/') {
		 
		return 2;
	}

	 
	 
	for (i = 0; abs_uri_protocols[i].proto != NULL; i++) {
		if (mg_strncasecmp(uri,
		                   abs_uri_protocols[i].proto,
		                   abs_uri_protocols[i].proto_len) == 0) {

			hostend = strchr(uri + abs_uri_protocols[i].proto_len, '/');
			if (!hostend) {
				return 0;
			}
			portbegin = strchr(uri + abs_uri_protocols[i].proto_len, ':');
			if (!portbegin) {
				return 3;
			}

			port = strtoul(portbegin + 1, &portend, 10);
			if ((portend != hostend) || (port <= 0) || !is_valid_port(port)) {
				return 0;
			}

			return 4;
		}
	}

	return 0;
}
