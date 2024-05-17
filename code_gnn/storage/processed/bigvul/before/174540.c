route_netmask(uint32_t ip_in)
{
  
 uint32_t p = ntohl(ip_in);
 uint32_t t;

 if (IN_CLASSA(p))
		t = ~IN_CLASSA_NET;
 else {
 if (IN_CLASSB(p))
			t = ~IN_CLASSB_NET;
 else {
 if (IN_CLASSC(p))
				t = ~IN_CLASSC_NET;
 else
				t = 0;
 }
 }

 while (t & p)
		t >>= 1;

 return (htonl(~t));
}
