read_lease(const struct interface *iface)
{
 int fd;
 struct dhcp_message *dhcp;
 ssize_t bytes;

	fd = open(iface->leasefile, O_RDONLY);
 if (fd == -1) {
 if (errno != ENOENT)
			syslog(LOG_ERR, "%s: open `%s': %m",
			    iface->name, iface->leasefile);
 return NULL;
 }
	syslog(LOG_DEBUG, "%s: reading lease `%s'",
	    iface->name, iface->leasefile);
	dhcp = xmalloc(sizeof(*dhcp));
	memset(dhcp, 0, sizeof(*dhcp));
	bytes = read(fd, dhcp, sizeof(*dhcp));
	close(fd);
 if (bytes < 0) {
		free(dhcp);
		dhcp = NULL;
 }
 return dhcp;
}
