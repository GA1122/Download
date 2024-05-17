print_options(void)
{
 const struct dhcp_opt *opt;
 const char **p;

 for (p = if_params; *p; p++)
		printf(" -  %s\n", *p);

 for (p = dhcp_params; *p; p++)
		printf("    %s\n", *p);

 for (opt = dhcp_opts; opt->option; opt++)
 if (opt->var)
			printf("%03d %s\n", opt->option, opt->var);
}
