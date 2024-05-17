snap_print(netdissect_options *ndo, const u_char *p, u_int length, u_int caplen,
	const struct lladdr_info *src, const struct lladdr_info *dst,
	u_int bridge_pad)
{
	uint32_t orgcode;
	register u_short et;
	register int ret;

	ND_TCHECK2(*p, 5);
	if (caplen < 5 || length < 5)
		goto trunc;
	orgcode = EXTRACT_24BITS(p);
	et = EXTRACT_16BITS(p + 3);

	if (ndo->ndo_eflag) {
		 
		ND_PRINT((ndo, "oui %s (0x%06x), %s %s (0x%04x), length %u: ",
		     tok2str(oui_values, "Unknown", orgcode),
		     orgcode,
		     (orgcode == 0x000000 ? "ethertype" : "pid"),
		     tok2str(oui_to_struct_tok(orgcode), "Unknown", et),
		     et, length - 5));
	}
	p += 5;
	length -= 5;
	caplen -= 5;

	switch (orgcode) {
	case OUI_ENCAP_ETHER:
	case OUI_CISCO_90:
		 
		ret = ethertype_print(ndo, et, p, length, caplen, src, dst);
		if (ret)
			return (ret);
		break;

	case OUI_APPLETALK:
		if (et == ETHERTYPE_ATALK) {
			 
			ret = ethertype_print(ndo, et, p, length, caplen, src, dst);
			if (ret)
				return (ret);
		}
		break;

	case OUI_CISCO:
                switch (et) {
                case PID_CISCO_CDP:
                        cdp_print(ndo, p, length, caplen);
                        return (1);
                case PID_CISCO_DTP:
                        dtp_print(ndo, p, length);
                        return (1);
                case PID_CISCO_UDLD:
                        udld_print(ndo, p, length);
                        return (1);
                case PID_CISCO_VTP:
                        vtp_print(ndo, p, length);
                        return (1);
                case PID_CISCO_PVST:
                case PID_CISCO_VLANBRIDGE:
                        stp_print(ndo, p, length);
                        return (1);
                default:
                        break;
                }
		break;

	case OUI_RFC2684:
		switch (et) {

		case PID_RFC2684_ETH_FCS:
		case PID_RFC2684_ETH_NOFCS:
			 
			 
			ND_TCHECK2(*p, bridge_pad);
			caplen -= bridge_pad;
			length -= bridge_pad;
			p += bridge_pad;

			 
			ether_print(ndo, p, length, caplen, NULL, NULL);
			return (1);

		case PID_RFC2684_802_5_FCS:
		case PID_RFC2684_802_5_NOFCS:
			 
			 
			ND_TCHECK2(*p, bridge_pad);
			caplen -= bridge_pad;
			length -= bridge_pad;
			p += bridge_pad;

			 
			token_print(ndo, p, length, caplen);
			return (1);

		case PID_RFC2684_FDDI_FCS:
		case PID_RFC2684_FDDI_NOFCS:
			 
			 
			ND_TCHECK2(*p, bridge_pad + 1);
			caplen -= bridge_pad + 1;
			length -= bridge_pad + 1;
			p += bridge_pad + 1;

			 
			fddi_print(ndo, p, length, caplen);
			return (1);

		case PID_RFC2684_BPDU:
			stp_print(ndo, p, length);
			return (1);
		}
	}
	if (!ndo->ndo_eflag) {
		 
		if (src != NULL && dst != NULL) {
			ND_PRINT((ndo, "%s > %s ",
				(src->addr_string)(ndo, src->addr),
				(dst->addr_string)(ndo, dst->addr)));
		}
		 
		if (orgcode == 0x000000) {
			ND_PRINT((ndo, "SNAP, ethertype %s (0x%04x), length %u: ",
			     tok2str(ethertype_values, "Unknown", et),
			     et, length));
		} else {
			ND_PRINT((ndo, "SNAP, oui %s (0x%06x), pid %s (0x%04x), length %u: ",
			     tok2str(oui_values, "Unknown", orgcode),
			     orgcode,
			     tok2str(oui_to_struct_tok(orgcode), "Unknown", et),
			     et, length));
		}
	}
	return (0);

trunc:
	ND_PRINT((ndo, "[|snap]"));
	return (1);
}
