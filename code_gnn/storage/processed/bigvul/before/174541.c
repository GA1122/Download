write_lease(const struct interface *iface, const struct dhcp_message *dhcp)
{
 int fd;
 ssize_t bytes = sizeof(*dhcp);
 const uint8_t *p = dhcp->options;
 const uint8_t *e = p + sizeof(dhcp->options);
 uint8_t l;
 uint8_t o = 0;

  
 if (is_bootp(dhcp)) {
		unlink(iface->leasefile);
 return 0;
 }

	syslog(LOG_DEBUG, "%s: writing lease `%s'",
	    iface->name, iface->leasefile);

	fd = open(iface->leasefile, O_WRONLY | O_CREAT | O_TRUNC, 0444);
#ifdef ANDROID
 if (fd == -1 && errno == EACCES) {
  
		unlink(iface->leasefile);
		fd = open(iface->leasefile, O_WRONLY | O_CREAT | O_TRUNC, 0444);
 }
#endif
 if (fd == -1) {
		syslog(LOG_ERR, "%s: open: %m", iface->name);
 return -1;
 }

  
 while (p < e) {
		o = *p;
 if (o == DHO_END) {
			bytes = p - (const uint8_t *)dhcp;
 break;
 }
		p++;
 if (o != DHO_PAD) {
			l = *p++;
			p += l;
 }
 }
	bytes = write(fd, dhcp, bytes);
	close(fd);
 return bytes;
}
