initialize_proxy_configuration (const struct url *u, struct request *req,
                                struct url *proxy, char **proxyauth)
{
  char *proxy_user, *proxy_passwd;
   
  if (opt.proxy_user && opt.proxy_passwd)
    {
      proxy_user = opt.proxy_user;
      proxy_passwd = opt.proxy_passwd;
    }
  else
    {
      proxy_user = proxy->user;
      proxy_passwd = proxy->passwd;
    }
   
  if (proxy_user && proxy_passwd)
    *proxyauth = basic_authentication_encode (proxy_user, proxy_passwd);

   
#ifdef HAVE_SSL
  if (u->scheme != SCHEME_HTTPS)
#endif
    request_set_header (req, "Proxy-Authorization", *proxyauth, rel_value);
}
