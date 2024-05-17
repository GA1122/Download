zephyr_print(netdissect_options *ndo, const u_char *cp, int length)
{
    struct z_packet z;
    const char *parse = (const char *) cp;
    int parselen = length;
    const char *s;
    int lose = 0;

     

    z.kind = 0;
    z.class = 0;
    z.inst = 0;
    z.opcode = 0;
    z.sender = 0;
    z.recipient = 0;

#define PARSE_STRING				\
	s = parse_field(ndo, &parse, &parselen);	\
	if (!s) lose = 1;

#define PARSE_FIELD_INT(field)			\
	PARSE_STRING				\
	if (!lose) field = strtol(s, 0, 16);

#define PARSE_FIELD_STR(field)			\
	PARSE_STRING				\
	if (!lose) field = s;

    PARSE_FIELD_STR(z.version);
    if (lose) return;
    if (strncmp(z.version, "ZEPH", 4))
	return;

    PARSE_FIELD_INT(z.numfields);
    PARSE_FIELD_INT(z.kind);
    PARSE_FIELD_STR(z.uid);
    PARSE_FIELD_INT(z.port);
    PARSE_FIELD_INT(z.auth);
    PARSE_FIELD_INT(z.authlen);
    PARSE_FIELD_STR(z.authdata);
    PARSE_FIELD_STR(z.class);
    PARSE_FIELD_STR(z.inst);
    PARSE_FIELD_STR(z.opcode);
    PARSE_FIELD_STR(z.sender);
    PARSE_FIELD_STR(z.recipient);
    PARSE_FIELD_STR(z.format);
    PARSE_FIELD_INT(z.cksum);
    PARSE_FIELD_INT(z.multi);
    PARSE_FIELD_STR(z.multi_uid);

    if (lose) {
	ND_PRINT((ndo, " [|zephyr] (%d)", length));
	return;
    }

    ND_PRINT((ndo, " zephyr"));
    if (strncmp(z.version+4, "0.2", 3)) {
	ND_PRINT((ndo, " v%s", z.version+4));
	return;
    }

    ND_PRINT((ndo, " %s", tok2str(z_types, "type %d", z.kind)));
    if (z.kind == Z_PACKET_SERVACK) {
	 
	const char *ackdata = NULL;
	PARSE_FIELD_STR(ackdata);
	if (!lose && strcmp(ackdata, "SENT"))
	    ND_PRINT((ndo, "/%s", str_to_lower(ackdata)));
    }
    if (*z.sender) ND_PRINT((ndo, " %s", z.sender));

    if (!strcmp(z.class, "USER_LOCATE")) {
	if (!strcmp(z.opcode, "USER_HIDE"))
	    ND_PRINT((ndo, " hide"));
	else if (!strcmp(z.opcode, "USER_UNHIDE"))
	    ND_PRINT((ndo, " unhide"));
	else
	    ND_PRINT((ndo, " locate %s", z.inst));
	return;
    }

    if (!strcmp(z.class, "ZEPHYR_ADMIN")) {
	ND_PRINT((ndo, " zephyr-admin %s", str_to_lower(z.opcode)));
	return;
    }

    if (!strcmp(z.class, "ZEPHYR_CTL")) {
	if (!strcmp(z.inst, "CLIENT")) {
	    if (!strcmp(z.opcode, "SUBSCRIBE") ||
		!strcmp(z.opcode, "SUBSCRIBE_NODEFS") ||
		!strcmp(z.opcode, "UNSUBSCRIBE")) {

		ND_PRINT((ndo, " %ssub%s", strcmp(z.opcode, "SUBSCRIBE") ? "un" : "",
				   strcmp(z.opcode, "SUBSCRIBE_NODEFS") ? "" :
								   "-nodefs"));
		if (z.kind != Z_PACKET_SERVACK) {
		     
		    const char *c = NULL, *i = NULL, *r = NULL;
		    PARSE_FIELD_STR(c);
		    PARSE_FIELD_STR(i);
		    PARSE_FIELD_STR(r);
		    if (!lose) ND_PRINT((ndo, " %s", z_triple(c, i, r)));
		}
		return;
	    }

	    if (!strcmp(z.opcode, "GIMME")) {
		ND_PRINT((ndo, " ret"));
		return;
	    }

	    if (!strcmp(z.opcode, "GIMMEDEFS")) {
		ND_PRINT((ndo, " gimme-defs"));
		return;
	    }

	    if (!strcmp(z.opcode, "CLEARSUB")) {
		ND_PRINT((ndo, " clear-subs"));
		return;
	    }

	    ND_PRINT((ndo, " %s", str_to_lower(z.opcode)));
	    return;
	}

	if (!strcmp(z.inst, "HM")) {
	    ND_PRINT((ndo, " %s", str_to_lower(z.opcode)));
	    return;
	}

	if (!strcmp(z.inst, "REALM")) {
	    if (!strcmp(z.opcode, "ADD_SUBSCRIBE"))
		ND_PRINT((ndo, " realm add-subs"));
	    if (!strcmp(z.opcode, "REQ_SUBSCRIBE"))
		ND_PRINT((ndo, " realm req-subs"));
	    if (!strcmp(z.opcode, "RLM_SUBSCRIBE"))
		ND_PRINT((ndo, " realm rlm-sub"));
	    if (!strcmp(z.opcode, "RLM_UNSUBSCRIBE"))
		ND_PRINT((ndo, " realm rlm-unsub"));
	    return;
	}
    }

    if (!strcmp(z.class, "HM_CTL")) {
	ND_PRINT((ndo, " hm_ctl %s", str_to_lower(z.inst)));
	ND_PRINT((ndo, " %s", str_to_lower(z.opcode)));
	return;
    }

    if (!strcmp(z.class, "HM_STAT")) {
	if (!strcmp(z.inst, "HMST_CLIENT") && !strcmp(z.opcode, "GIMMESTATS")) {
	    ND_PRINT((ndo, " get-client-stats"));
	    return;
	}
    }

    if (!strcmp(z.class, "WG_CTL")) {
	ND_PRINT((ndo, " wg_ctl %s", str_to_lower(z.inst)));
	ND_PRINT((ndo, " %s", str_to_lower(z.opcode)));
	return;
    }

    if (!strcmp(z.class, "LOGIN")) {
	if (!strcmp(z.opcode, "USER_FLUSH")) {
	    ND_PRINT((ndo, " flush_locs"));
	    return;
	}

	if (!strcmp(z.opcode, "NONE") ||
	    !strcmp(z.opcode, "OPSTAFF") ||
	    !strcmp(z.opcode, "REALM-VISIBLE") ||
	    !strcmp(z.opcode, "REALM-ANNOUNCED") ||
	    !strcmp(z.opcode, "NET-VISIBLE") ||
	    !strcmp(z.opcode, "NET-ANNOUNCED")) {
	    ND_PRINT((ndo, " set-exposure %s", str_to_lower(z.opcode)));
	    return;
	}
    }

    if (!*z.recipient)
	z.recipient = "*";

    ND_PRINT((ndo, " to %s", z_triple(z.class, z.inst, z.recipient)));
    if (*z.opcode)
	ND_PRINT((ndo, " op %s", z.opcode));
}