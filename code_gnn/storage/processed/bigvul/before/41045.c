int _mkp_init(struct plugin_api **api, char *confdir)
{
    mk_api = *api;

     
    mk_list_init(&mk_secure_ip);
    mk_list_init(&mk_secure_url);
    mk_list_init(&mk_secure_deny_hotlink);

     
    mk_security_conf(confdir);
    return 0;
}
