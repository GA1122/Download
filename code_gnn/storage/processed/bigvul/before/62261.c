decode_mdt_vpn_nlri(netdissect_options *ndo,
                    const u_char *pptr, char *buf, u_int buflen)
{

    const u_char *rd;
    const u_char *vpn_ip;

    ND_TCHECK(pptr[0]);

     
    if (*pptr != MDT_VPN_NLRI_LEN * 8)
	return -1;
    pptr++;

     
    ND_TCHECK2(pptr[0], 8);
    rd = pptr;
    pptr+=8;

     
    ND_TCHECK2(pptr[0], sizeof(struct in_addr));
    vpn_ip = pptr;
    pptr+=sizeof(struct in_addr);

     
    ND_TCHECK2(pptr[0], sizeof(struct in_addr));

    snprintf(buf, buflen, "RD: %s, VPN IP Address: %s, MC Group Address: %s",
	     bgp_vpn_rd_print(ndo, rd), ipaddr_string(ndo, vpn_ip), ipaddr_string(ndo, pptr));

    return MDT_VPN_NLRI_LEN + 1;

 trunc:

return -2;
}
