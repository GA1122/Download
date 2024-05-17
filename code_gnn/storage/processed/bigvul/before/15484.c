http_loop (const struct url *u, struct url *original_url, char **newloc,
           char **local_file, const char *referer, int *dt, struct url *proxy,
           struct iri *iri)
{
  int count;
  bool got_head = false;          
  bool time_came_from_head = false;
  bool got_name = false;
  char *tms;
  const char *tmrate;
  uerr_t err, ret = TRYLIMEXC;
  time_t tmr = -1;                
  struct http_stat hstat;         
  struct stat st;
  bool send_head_first = true;
  bool force_full_retrieve = false;


   
  if (opt.warc_filename != NULL)
    force_full_retrieve = true;


   
  assert (local_file == NULL || *local_file == NULL);

   
  if (local_file && opt.output_document)
    *local_file = HYPHENP (opt.output_document) ? NULL : xstrdup (opt.output_document);

   
  *newloc = NULL;

   
  if (opt.cookies)
    load_cookies ();

   
  if (opt.ftp_glob && has_wildcards_p (u->path))
    logputs (LOG_VERBOSE, _("Warning: wildcards not supported in HTTP.\n"));

   
  xzero (hstat);
  hstat.referer = referer;

  if (opt.output_document)
    {
      hstat.local_file = xstrdup (opt.output_document);
      got_name = true;
    }
  else if (!opt.content_disposition)
    {
      hstat.local_file =
        url_file_name (opt.trustservernames ? u : original_url, NULL);
      got_name = true;
    }

  if (got_name && file_exists_p (hstat.local_file, NULL) && opt.noclobber && !opt.output_document)
    {
       
      get_file_flags (hstat.local_file, dt);
      ret = RETROK;
      goto exit;
    }

   
  count = 0;

   
  *dt = 0;

   
  if (!opt.spider)
    send_head_first = false;

   
  if (opt.content_disposition && opt.always_rest)
    send_head_first = true;

#ifdef HAVE_METALINK
  if (opt.metalink_over_http)
    {
      *dt |= METALINK_METADATA;
      send_head_first = true;
    }
#endif

  if (opt.timestamping)
    {
       
      if (opt.if_modified_since && !send_head_first && got_name && file_exists_p (hstat.local_file, NULL))
        {
          *dt |= IF_MODIFIED_SINCE;
          {
            uerr_t timestamp_err = set_file_timestamp (&hstat);
            if (timestamp_err != RETROK)
              return timestamp_err;
          }
        }
         
      else if (opt.content_disposition || file_exists_p (hstat.local_file, NULL))
        send_head_first = true;
    }

   
  do
    {
       
      ++count;
      sleep_between_retrievals (count);

       
      tms = datetime_str (time (NULL));

      if (opt.spider && !got_head)
        logprintf (LOG_VERBOSE,
			  _("Spider mode enabled. Check if remote file exists.\n"));

       
      if (opt.verbose)
        {
          char *hurl = url_string (u, URL_AUTH_HIDE_PASSWD);

          if (count > 1)
            {
              char tmp[256];
              sprintf (tmp, _("(try:%2d)"), count);
              logprintf (LOG_NOTQUIET, "--%s--  %s  %s\n",
                         tms, tmp, hurl);
            }
          else
            {
              logprintf (LOG_NOTQUIET, "--%s--  %s\n",
                         tms, hurl);
            }

#ifdef WINDOWS
          ws_changetitle (hurl);
#endif
          xfree (hurl);
        }

       
      if (send_head_first && !got_head)
        *dt |= HEAD_ONLY;
      else
        *dt &= ~HEAD_ONLY;

       
      if (force_full_retrieve)
        hstat.restval = hstat.len;
      else if (opt.start_pos >= 0)
        hstat.restval = opt.start_pos;
      else if (opt.always_rest
          && got_name
          && stat (hstat.local_file, &st) == 0
          && S_ISREG (st.st_mode))
         
        hstat.restval = st.st_size;
      else if (count > 1)
         
        hstat.restval = hstat.len;
      else
        hstat.restval = 0;

       
      if ((proxy && count > 1)         
          || !opt.allow_cache)         
        *dt |= SEND_NOCACHE;
      else
        *dt &= ~SEND_NOCACHE;

       
      err = gethttp (u, original_url, &hstat, dt, proxy, iri, count);

       
      tms = datetime_str (time (NULL));

       
      if (hstat.newloc)
        *newloc = xstrdup (hstat.newloc);

      switch (err)
        {
        case HERR: case HEOF: case CONSOCKERR:
        case CONERROR: case READERR: case WRITEFAILED:
        case RANGEERR: case FOPEN_EXCL_ERR: case GATEWAYTIMEOUT:
           
          printwhat (count, opt.ntry);
          xfree (hstat.message);
          xfree (hstat.error);
          continue;
        case FWRITEERR: case FOPENERR:
           
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Cannot write to %s (%s).\n"),
                     quote (hstat.local_file), strerror (errno));
        case HOSTERR: case CONIMPOSSIBLE: case PROXERR: case SSLINITFAILED:
        case CONTNOTSUPPORTED: case VERIFCERTERR: case FILEBADFILE:
        case UNKNOWNATTR:
           
          ret = err;
          goto exit;
        case ATTRMISSING:
           
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Required attribute missing from Header received.\n"));
          ret = err;
          goto exit;
        case AUTHFAILED:
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Username/Password Authentication Failed.\n"));
          ret = err;
          goto exit;
        case WARC_ERR:
           
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Cannot write to WARC file.\n"));
          ret = err;
          goto exit;
        case WARC_TMP_FOPENERR: case WARC_TMP_FWRITEERR:
           
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Cannot write to temporary WARC file.\n"));
          ret = err;
          goto exit;
        case CONSSLERR:
           
          logprintf (LOG_NOTQUIET, _("Unable to establish SSL connection.\n"));
          ret = err;
          goto exit;
        case UNLINKERR:
           
          logputs (LOG_VERBOSE, "\n");
          logprintf (LOG_NOTQUIET, _("Cannot unlink %s (%s).\n"),
                     quote (hstat.local_file), strerror (errno));
          ret = err;
          goto exit;
        case NEWLOCATION:
        case NEWLOCATION_KEEP_POST:
           
          if (!*newloc)
            {
              logprintf (LOG_NOTQUIET,
                         _("ERROR: Redirection (%d) without location.\n"),
                         hstat.statcode);
              ret = WRONGCODE;
            }
          else
            {
              ret = err;
            }
          goto exit;
        case RETRUNNEEDED:
           
          ret = RETROK;
          goto exit;
        case RETRFINISHED:
           
          break;
#ifdef HAVE_METALINK
        case RETR_WITH_METALINK:
          {
            if (hstat.metalink == NULL)
              {
                logputs (LOG_NOTQUIET,
                         _("Could not find Metalink data in HTTP response. "
                           "Downloading file using HTTP GET.\n"));
                *dt &= ~METALINK_METADATA;
                *dt &= ~HEAD_ONLY;
                got_head = true;
                continue;
              }

            logputs (LOG_VERBOSE,
                     _("Metalink headers found. "
                       "Switching to Metalink mode.\n"));

            ret = retrieve_from_metalink (hstat.metalink);
            goto exit;
          }
          break;
#endif
        default:
           
          abort ();
        }

      if (!(*dt & RETROKF))
        {
          char *hurl = NULL;
          if (!opt.verbose)
            {
               
              hurl = url_string (u, URL_AUTH_HIDE_PASSWD);
              logprintf (LOG_NONVERBOSE, "%s:\n", hurl);
            }

           
          if (*dt & HEAD_ONLY
              && (hstat.statcode == 500 || hstat.statcode == 501))
            {
              got_head = true;
              continue;
            }
           
          else if (opt.spider && !iri->utf8_encode)
            {
               
              if (!hurl)
                hurl = url_string (u, URL_AUTH_HIDE_PASSWD);
              nonexisting_url (hurl);
              logprintf (LOG_NOTQUIET, _("\
Remote file does not exist -- broken link!!!\n"));
            }
          else if (check_retry_on_http_error (hstat.statcode))
            {
              printwhat (count, opt.ntry);
              continue;
            }
          else
            {
              logprintf (LOG_NOTQUIET, _("%s ERROR %d: %s.\n"),
                         tms, hstat.statcode,
                         quotearg_style (escape_quoting_style, hstat.error));
            }
          logputs (LOG_VERBOSE, "\n");
          ret = WRONGCODE;
          xfree (hurl);
          goto exit;
        }

       
      if (!got_head || (opt.spider && !opt.recursive))
        {
          got_head = true;     

          if (opt.timestamping && !hstat.remote_time)
            {
              logputs (LOG_NOTQUIET, _("\
Last-modified header missing -- time-stamps turned off.\n"));
            }
          else if (hstat.remote_time)
            {
               
              tmr = http_atotm (hstat.remote_time);
              if (tmr == (time_t) (-1))
                logputs (LOG_VERBOSE, _("\
Last-modified header invalid -- time-stamp ignored.\n"));
              if (*dt & HEAD_ONLY)
                time_came_from_head = true;
            }

          if (send_head_first)
            {
               
              if (opt.timestamping)
                {
                  if (hstat.orig_file_name)  
                    {
                      if (hstat.remote_time &&
                          tmr != (time_t) (-1))
                        {
                           
                          if (hstat.orig_file_tstamp >= tmr)
                            {
                              if (hstat.contlen == -1
                                  || hstat.orig_file_size == hstat.contlen)
                                {
                                  logprintf (LOG_VERBOSE, _("\
Server file no newer than local file %s -- not retrieving.\n\n"),
                                             quote (hstat.orig_file_name));
                                  ret = RETROK;
                                  goto exit;
                                }
                              else
                                {
                                  logprintf (LOG_VERBOSE, _("\
The sizes do not match (local %s) -- retrieving.\n"),
                                             number_to_static_string (hstat.orig_file_size));
                                }
                            }
                          else
                            {
                              force_full_retrieve = true;
                              logputs (LOG_VERBOSE,
                                       _("Remote file is newer, retrieving.\n"));
                            }

                          logputs (LOG_VERBOSE, "\n");
                        }
                    }

                   
                  hstat.timestamp_checked = true;
                }

              if (opt.spider)
                {
                  bool finished = true;
                  if (opt.recursive)
                    {
                      if ((*dt & TEXTHTML) || (*dt & TEXTCSS))
                        {
                          logputs (LOG_VERBOSE, _("\
Remote file exists and could contain links to other resources -- retrieving.\n\n"));
                          finished = false;
                        }
                      else
                        {
                          logprintf (LOG_VERBOSE, _("\
Remote file exists but does not contain any link -- not retrieving.\n\n"));
                          ret = RETROK;  
                        }
                    }
                  else
                    {
                      if ((*dt & TEXTHTML) || (*dt & TEXTCSS))
                        {
                          logprintf (LOG_VERBOSE, _("\
Remote file exists and could contain further links,\n\
but recursion is disabled -- not retrieving.\n\n"));
                        }
                      else
                        {
                          logprintf (LOG_VERBOSE, _("\
Remote file exists.\n\n"));
                        }
                      ret = RETROK;  
                    }

                  if (finished)
                    {
                      logprintf (LOG_NONVERBOSE,
                                 _("%s URL: %s %2d %s\n"),
                                 tms, u->url, hstat.statcode,
                                 hstat.message ? quotearg_style (escape_quoting_style, hstat.message) : "");
                      goto exit;
                    }
                }

              got_name = true;
              *dt &= ~HEAD_ONLY;
              count = 0;           
              xfree (hstat.message);
              xfree (hstat.error);
              continue;
            }  
        }  

      if (opt.useservertimestamps
          && (tmr != (time_t) (-1))
          && ((hstat.len == hstat.contlen) ||
              ((hstat.res == 0) && (hstat.contlen == -1))))
        {
          const char *fl = NULL;
          set_local_file (&fl, hstat.local_file);
          if (fl)
            {
              time_t newtmr = -1;
               
              if (time_came_from_head
                  && hstat.remote_time && hstat.remote_time[0])
                {
                  newtmr = http_atotm (hstat.remote_time);
                  if (newtmr != (time_t)-1)
                    tmr = newtmr;
                }
              touch (fl, tmr);
            }
        }
       

      tmrate = retr_rate (hstat.rd_size, hstat.dltime);
      total_download_time += hstat.dltime;

      if (hstat.len == hstat.contlen)
        {
          if (*dt & RETROKF || opt.content_on_error)
            {
              bool write_to_stdout = (opt.output_document && HYPHENP (opt.output_document));

              logprintf (LOG_VERBOSE,
                         write_to_stdout
                         ? _("%s (%s) - written to stdout %s[%s/%s]\n\n")
                         : _("%s (%s) - %s saved [%s/%s]\n\n"),
                         tms, tmrate,
                         write_to_stdout ? "" : quote (hstat.local_file),
                         number_to_static_string (hstat.len),
                         number_to_static_string (hstat.contlen));
              logprintf (LOG_NONVERBOSE,
                         "%s URL:%s [%s/%s] -> \"%s\" [%d]\n",
                         tms, u->url,
                         number_to_static_string (hstat.len),
                         number_to_static_string (hstat.contlen),
                         hstat.local_file, count);
            }
          ++numurls;
          total_downloaded_bytes += hstat.rd_size;

           
          if (*dt & ADDED_HTML_EXTENSION)
            downloaded_file (FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED, hstat.local_file);
          else
            downloaded_file (FILE_DOWNLOADED_NORMALLY, hstat.local_file);

          ret = RETROK;
          goto exit;
        }
      else if (hstat.res == 0)  
        {
          if (hstat.contlen == -1)   
            {
              if (*dt & RETROKF || opt.content_on_error)
                {
                  bool write_to_stdout = (opt.output_document && HYPHENP (opt.output_document));

                  logprintf (LOG_VERBOSE,
                             write_to_stdout
                             ? _("%s (%s) - written to stdout %s[%s]\n\n")
                             : _("%s (%s) - %s saved [%s]\n\n"),
                             tms, tmrate,
                             write_to_stdout ? "" : quote (hstat.local_file),
                             number_to_static_string (hstat.len));
                  logprintf (LOG_NONVERBOSE,
                             "%s URL:%s [%s] -> \"%s\" [%d]\n",
                             tms, u->url, number_to_static_string (hstat.len),
                             hstat.local_file, count);
                }
              ++numurls;
              total_downloaded_bytes += hstat.rd_size;

               
              if (*dt & ADDED_HTML_EXTENSION)
                downloaded_file (FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED, hstat.local_file);
              else
                downloaded_file (FILE_DOWNLOADED_NORMALLY, hstat.local_file);

              ret = RETROK;
              goto exit;
            }
          else if (hstat.len < hstat.contlen)  
            {
              logprintf (LOG_VERBOSE,
                         _("%s (%s) - Connection closed at byte %s. "),
                         tms, tmrate, number_to_static_string (hstat.len));
              printwhat (count, opt.ntry);
              continue;
            }
          else if (hstat.len != hstat.restval)
             
            abort ();
          else
            {
               
              ret = RETROK;
              goto exit;
            }
        }
      else  
        {
          if (hstat.contlen == -1)
            {
              logprintf (LOG_VERBOSE,
                         _("%s (%s) - Read error at byte %s (%s)."),
                         tms, tmrate, number_to_static_string (hstat.len),
                         hstat.rderrmsg);
              printwhat (count, opt.ntry);
              continue;
            }
          else  
            {
              logprintf (LOG_VERBOSE,
                         _("%s (%s) - Read error at byte %s/%s (%s). "),
                         tms, tmrate,
                         number_to_static_string (hstat.len),
                         number_to_static_string (hstat.contlen),
                         hstat.rderrmsg);
              printwhat (count, opt.ntry);
              continue;
            }
        }
       
    }
  while (!opt.ntry || (count < opt.ntry));

exit:
  if ((ret == RETROK || opt.content_on_error) && local_file)
    {
      xfree (*local_file);
       
      if (hstat.local_file)
        *local_file = xstrdup (hstat.local_file);
    }
  free_hstat (&hstat);

  return ret;
}
