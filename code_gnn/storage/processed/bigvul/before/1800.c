static int name_from_dns_search(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family)
{
	char search[256];
	struct resolvconf conf;
	size_t l, dots;
	char *p, *z;

	if (__get_resolv_conf(&conf, search, sizeof search) < 0) return -1;

	 
	for (dots=l=0; name[l]; l++) if (name[l]=='.') dots++;
	if (dots >= conf.ndots || name[l-1]=='.') *search = 0;

	 
	if (l >= 256) return EAI_NONAME;

	 
	memcpy(canon, name, l);
	canon[l] = '.';

	for (p=search; *p; p=z) {
		for (; isspace(*p); p++);
		for (z=p; *z && !isspace(*z); z++);
		if (z==p) break;
		if (z-p < 256 - l - 1) {
			memcpy(canon+l+1, p, z-p);
			canon[z-p+1+l] = 0;
			int cnt = name_from_dns(buf, canon, canon, family, &conf);
			if (cnt) return cnt;
		}
	}

	canon[l] = 0;
	return name_from_dns(buf, canon, name, family, &conf);
}
