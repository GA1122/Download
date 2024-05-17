static struct Packet *construct_packet(Ssh ssh, int pkttype, va_list ap)
{
    int argtype;
    Bignum bn;
    struct Packet *pkt;

    pkt = ssh1_pkt_init(pkttype);

    while ((argtype = va_arg(ap, int)) != PKT_END) {
	unsigned char *argp, argchar;
	char *sargp;
	unsigned long argint;
	int arglen;
	switch (argtype) {
	   
	  case PKT_INT:
	    argint = va_arg(ap, int);
	    ssh_pkt_adduint32(pkt, argint);
	    break;
	  case PKT_CHAR:
	    argchar = (unsigned char) va_arg(ap, int);
	    ssh_pkt_addbyte(pkt, argchar);
	    break;
	  case PKT_DATA:
	    argp = va_arg(ap, unsigned char *);
	    arglen = va_arg(ap, int);
	    ssh_pkt_adddata(pkt, argp, arglen);
	    break;
	  case PKT_STR:
	    sargp = va_arg(ap, char *);
	    ssh_pkt_addstring(pkt, sargp);
	    break;
	  case PKT_BIGNUM:
	    bn = va_arg(ap, Bignum);
	    ssh1_pkt_addmp(pkt, bn);
	    break;
	}
    }

    return pkt;
}
