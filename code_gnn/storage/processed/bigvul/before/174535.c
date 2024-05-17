int make_option_mask(uint8_t *mask, const char *opts, int add)
{
 char *token, *o, *p, *t;
 const struct dhcp_opt *opt;
 int match, n;

	o = p = xstrdup(opts);
 while ((token = strsep(&p, ", "))) {
 if (*token == '\0')
 continue;
 for (opt = dhcp_opts; opt->option; opt++) {
 if (!opt->var)
 continue;
			match = 0;
 if (strcmp(opt->var, token) == 0)
				match = 1;
 else {
				errno = 0;
				n = strtol(token, &t, 0);
 if (errno == 0 && !*t)
 if (opt->option == n)
						match = 1;
 }
 if (match) {
 if (add == 2 && !(opt->type & IPV4)) {
					free(o);
					errno = EINVAL;
 return -1;
 }
 if (add == 1 || add == 2)
					add_option_mask(mask,
					    opt->option);
 else
					del_option_mask(mask,
					    opt->option);
 break;
 }
 }
 if (!opt->option) {
			free(o);
			errno = ENOENT;
 return -1;
 }
 }
	free(o);
 return 0;
}
