txtproto_print(netdissect_options *ndo, const u_char *pptr, u_int len,
    const char *protoname, const char **cmds, u_int flags)
{
	u_int idx, eol;
	u_char token[MAX_TOKEN+1];
	const char *cmd;
	int is_reqresp = 0;
	const char *pnp;

	if (cmds != NULL) {
		 
		idx = fetch_token(ndo, pptr, 0, len, token, sizeof(token));
		if (idx != 0) {
			 
			while ((cmd = *cmds++) != NULL) {
				if (ascii_strcasecmp((const char *)token, cmd) == 0) {
					 
					is_reqresp = 1;
					break;
				}
			}

			 
			if (flags & RESP_CODE_SECOND_TOKEN) {
				 
				idx = fetch_token(ndo, pptr, idx, len, token,
				    sizeof(token));
			}
			if (idx != 0) {
				if (isdigit(token[0]) && isdigit(token[1]) &&
				    isdigit(token[2]) && token[3] == '\0') {
					 
					is_reqresp = 1;
				}
			}
		}
	} else {
		 
		is_reqresp = 1;
	}

	 
	for (pnp = protoname; *pnp != '\0'; pnp++)
		ND_PRINT((ndo, "%c", toupper((u_char)*pnp)));

	if (is_reqresp) {
		 
		if (ndo->ndo_vflag) {
			 
			ND_PRINT((ndo, ", length: %u", len));
			for (idx = 0;
			    idx < len && (eol = print_txt_line(ndo, protoname, "\n\t", pptr, idx, len)) != 0;
			    idx = eol)
				;
		} else {
			 
			print_txt_line(ndo, protoname, ": ", pptr, 0, len);
		}
	}
}
