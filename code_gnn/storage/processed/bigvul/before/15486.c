initialize_request (const struct url *u, struct http_stat *hs, int *dt, struct url *proxy,
                    bool inhibit_keep_alive, bool *basic_auth_finished,
                    wgint *body_data_size, char **user, char **passwd, uerr_t *ret)
{
  bool head_only = !!(*dt & HEAD_ONLY);
  struct request *req;

   
  {
    char *meth_arg;
    const char *meth = "GET";
    if (head_only)
      meth = "HEAD";
    else if (opt.method)
      meth = opt.method;
     
    if (proxy
#ifdef HAVE_SSL
         
        && u->scheme != SCHEME_HTTPS
#endif
        )
      meth_arg = xstrdup (u->url);
    else
      meth_arg = url_full_path (u);
    req = request_new (meth, meth_arg);
  }

  request_set_header (req, "Referer", (char *) hs->referer, rel_none);
  if (*dt & SEND_NOCACHE)
    {
       
      request_set_header (req, "Cache-Control", "no-cache", rel_none);

       
      request_set_header (req, "Pragma", "no-cache", rel_none);
    }
  if (*dt & IF_MODIFIED_SINCE)
    {
      char strtime[32];
      uerr_t err = time_to_rfc1123 (hs->orig_file_tstamp, strtime, countof (strtime));

      if (err != RETROK)
        {
          logputs (LOG_VERBOSE, _("Cannot convert timestamp to http format. "
                                  "Falling back to time 0 as last modification "
                                  "time.\n"));
          strcpy (strtime, "Thu, 01 Jan 1970 00:00:00 GMT");
        }
      request_set_header (req, "If-Modified-Since", xstrdup (strtime), rel_value);
    }
  if (hs->restval)
    request_set_header (req, "Range",
                        aprintf ("bytes=%s-",
                                 number_to_static_string (hs->restval)),
                        rel_value);
  SET_USER_AGENT (req);
  request_set_header (req, "Accept", "*/*", rel_none);
#ifdef HAVE_LIBZ
  if (opt.compression != compression_none)
    request_set_header (req, "Accept-Encoding", "gzip", rel_none);
  else
#endif
    request_set_header (req, "Accept-Encoding", "identity", rel_none);

   
  if (u->user)
    *user = u->user;
  else if (opt.user && (opt.use_askpass || opt.ask_passwd))
    *user = opt.user;
  else if (opt.http_user)
    *user = opt.http_user;
  else if (opt.user)
    *user = opt.user;
  else
    *user = NULL;

   
  if (u->passwd)
    *passwd = u->passwd;
  else if (opt.passwd && (opt.use_askpass || opt.ask_passwd))
    *passwd = opt.passwd;
  else if (opt.http_passwd)
    *passwd = opt.http_passwd;
  else if (opt.passwd)
    *passwd = opt.passwd;
  else
    *passwd = NULL;

   
  if (opt.netrc && (!*user || !*passwd))
    search_netrc (u->host, (const char **) user, (const char **) passwd, 0, NULL);

   
  if (*user && *passwd && (!u->user || opt.auth_without_challenge))
    {
       
      *basic_auth_finished = maybe_send_basic_creds (u->host, *user, *passwd, req);
    }

   
  {
     
    static const char *hfmt[][2] = {
      { "%s", "[%s]" }, { "%s:%d", "[%s]:%d" }
    };
    int add_port = u->port != scheme_default_port (u->scheme);
    int add_squares = strchr (u->host, ':') != NULL;
    request_set_header (req, "Host",
                        aprintf (hfmt[add_port][add_squares], u->host, u->port),
                        rel_value);
  }

  if (inhibit_keep_alive)
    request_set_header (req, "Connection", "Close", rel_none);
  else
    {
      request_set_header (req, "Connection", "Keep-Alive", rel_none);
      if (proxy)
        request_set_header (req, "Proxy-Connection", "Keep-Alive", rel_none);
    }

  if (opt.method)
    {

      if (opt.body_data || opt.body_file)
        {
          request_set_header (req, "Content-Type",
                              "application/x-www-form-urlencoded", rel_none);

          if (opt.body_data)
            *body_data_size = strlen (opt.body_data);
          else
            {
              *body_data_size = file_size (opt.body_file);
              if (*body_data_size == -1)
                {
                  logprintf (LOG_NOTQUIET, _("BODY data file %s missing: %s\n"),
                             quote (opt.body_file), strerror (errno));
                  request_free (&req);
                  *ret = FILEBADFILE;
                  return NULL;
                }
            }
          request_set_header (req, "Content-Length",
                              xstrdup (number_to_static_string (*body_data_size)),
                              rel_value);
        }
      else if (c_strcasecmp (opt.method, "post") == 0
               || c_strcasecmp (opt.method, "put") == 0
               || c_strcasecmp (opt.method, "patch") == 0)
        request_set_header (req, "Content-Length", "0", rel_none);
    }
  return req;
}
