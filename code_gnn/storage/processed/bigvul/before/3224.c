retrieve_url (struct url * orig_parsed, const char *origurl, char **file,
              char **newloc, const char *refurl, int *dt, bool recursive,
              struct iri *iri, bool register_status)
{
  uerr_t result;
  char *url;
  bool location_changed;
  bool iri_fallbacked = 0;
  int dummy;
  char *mynewloc, *proxy;
  struct url *u = orig_parsed, *proxy_url;
  int up_error_code;             
  char *local_file = NULL;
  int redirection_count = 0;

  bool method_suspended = false;
  char *saved_body_data = NULL;
  char *saved_method = NULL;
  char *saved_body_file_name = NULL;

   
  if (!dt)
    {
      dt = &dummy;
      dummy = 0;
    }
  url = xstrdup (origurl);
  if (newloc)
    *newloc = NULL;
  if (file)
    *file = NULL;

  if (!refurl)
    refurl = opt.referer;

 redirected:
   

  result = NOCONERROR;
  mynewloc = NULL;
  xfree(local_file);
  proxy_url = NULL;

  proxy = getproxy (u);
  if (proxy)
    {
      struct iri *pi = iri_new ();
      set_uri_encoding (pi, opt.locale, true);
      pi->utf8_encode = false;

       
      proxy_url = url_parse (proxy, &up_error_code, pi, true);
      if (!proxy_url)
        {
          char *error = url_error (proxy, up_error_code);
          logprintf (LOG_NOTQUIET, _("Error parsing proxy URL %s: %s.\n"),
                     proxy, error);
          xfree (url);
          xfree (error);
          xfree (proxy);
          iri_free (pi);
          RESTORE_METHOD;
          result = PROXERR;
          goto bail;
        }
      if (proxy_url->scheme != SCHEME_HTTP && proxy_url->scheme != u->scheme)
        {
          logprintf (LOG_NOTQUIET, _("Error in proxy URL %s: Must be HTTP.\n"), proxy);
          url_free (proxy_url);
          xfree (url);
          xfree (proxy);
          iri_free (pi);
          RESTORE_METHOD;
          result = PROXERR;
          goto bail;
        }
      iri_free(pi);
      xfree (proxy);
    }

  if (u->scheme == SCHEME_HTTP
#ifdef HAVE_SSL
      || u->scheme == SCHEME_HTTPS
#endif
      || (proxy_url && proxy_url->scheme == SCHEME_HTTP))
    {
#ifdef HAVE_HSTS
#ifdef TESTING
       
      hsts_store_t hsts_store = NULL;
#else
      extern hsts_store_t hsts_store;
#endif

      if (opt.hsts && hsts_store)
	{
	  if (hsts_match (hsts_store, u))
	    logprintf (LOG_VERBOSE, "URL transformed to HTTPS due to an HSTS policy\n");
	}
#endif
      result = http_loop (u, orig_parsed, &mynewloc, &local_file, refurl, dt,
                          proxy_url, iri);
    }
  else if (u->scheme == SCHEME_FTP
#ifdef HAVE_SSL
      || u->scheme == SCHEME_FTPS
#endif
      )
    {
       
      bool oldrec = recursive, glob = opt.ftp_glob;
      if (redirection_count)
        oldrec = glob = false;

      result = ftp_loop (u, orig_parsed, &local_file, dt, proxy_url,
                         recursive, glob);
      recursive = oldrec;

       
      if (redirection_count && local_file && (u->scheme == SCHEME_FTP
#ifdef HAVE_SSL
          || u->scheme == SCHEME_FTPS
#endif
          ))
        {
          if (has_html_suffix_p (local_file))
            *dt |= TEXTHTML;
        }
    }

  if (proxy_url)
    {
      url_free (proxy_url);
      proxy_url = NULL;
    }

  location_changed = (result == NEWLOCATION || result == NEWLOCATION_KEEP_POST);
  if (location_changed)
    {
      char *construced_newloc;
      struct url *newloc_parsed;

      assert (mynewloc != NULL);

      xfree (local_file);

       
      construced_newloc = uri_merge (url, mynewloc ? mynewloc : "");
      xfree (mynewloc);
      mynewloc = construced_newloc;

#ifdef ENABLE_IRI
       
      iri->utf8_encode = opt.enable_iri;
      if (opt.encoding_remote)
       set_uri_encoding (iri, opt.encoding_remote, true);
      set_content_encoding (iri, NULL);
      xfree (iri->orig_url);
#endif

       
      newloc_parsed = url_parse (mynewloc, &up_error_code, iri, true);
      if (!newloc_parsed)
        {
          char *error = url_error (mynewloc, up_error_code);
          logprintf (LOG_NOTQUIET, "%s: %s.\n", escnonprint_uri (mynewloc),
                     error);
          if (orig_parsed != u)
            {
              url_free (u);
            }
          xfree (url);
          xfree (mynewloc);
          xfree (error);
          RESTORE_METHOD;
          goto bail;
        }

       
      xfree (mynewloc);
      mynewloc = xstrdup (newloc_parsed->url);

       
      if (++redirection_count > opt.max_redirect)
        {
          logprintf (LOG_NOTQUIET, _("%d redirections exceeded.\n"),
                     opt.max_redirect);
          url_free (newloc_parsed);
          if (orig_parsed != u)
            {
              url_free (u);
            }
          xfree (url);
          xfree (mynewloc);
          RESTORE_METHOD;
          result = WRONGCODE;
          goto bail;
        }

      xfree (url);
      url = mynewloc;
      if (orig_parsed != u)
        {
          url_free (u);
        }
      u = newloc_parsed;

       
      if (result != NEWLOCATION_KEEP_POST && !method_suspended)
        SUSPEND_METHOD;

      goto redirected;
    }
  else
    {
      xfree(mynewloc);
    }

   
  if (!(*dt & RETROKF) && iri->utf8_encode)
    {
      iri->utf8_encode = false;
      if (orig_parsed != u)
        {
          url_free (u);
        }
      u = url_parse (origurl, NULL, iri, true);
      if (u)
        {
          if (strcmp(u->url, orig_parsed->url))
            {
              DEBUGP (("[IRI fallbacking to non-utf8 for %s\n", quote (url)));
              xfree (url);
              url = xstrdup (u->url);
              iri_fallbacked = 1;
              goto redirected;
            }
          else
              DEBUGP (("[Needn't fallback to non-utf8 for %s\n", quote (url)));
        }
      else
          DEBUGP (("[Couldn't fallback to non-utf8 for %s\n", quote (url)));
    }

  if (local_file && u && (*dt & RETROKF || opt.content_on_error))
    {
      register_download (u->url, local_file);

      if (!opt.spider && redirection_count && 0 != strcmp (origurl, u->url))
        register_redirection (origurl, u->url);

      if (*dt & TEXTHTML)
        register_html (local_file);

      if (*dt & TEXTCSS)
        register_css (local_file);
    }

  if (file)
    *file = local_file ? local_file : NULL;
  else
    xfree (local_file);

  if (orig_parsed != u)
    {
      url_free (u);
    }

  if (redirection_count || iri_fallbacked)
    {
      if (newloc)
        *newloc = url;
      else
        xfree (url);
    }
  else
    {
      if (newloc)
        *newloc = NULL;
      xfree (url);
    }

  RESTORE_METHOD;

bail:
  if (register_status)
    inform_exit_status (result);

  return result;
}
