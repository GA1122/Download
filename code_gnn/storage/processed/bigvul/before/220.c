ftp_loop_internal (struct url *u, struct fileinfo *f, ccon *con, char **local_file,
                   bool force_full_retrieve)
{
  int count, orig_lp;
  wgint restval, len = 0, qtyread = 0;
  char *tms, *locf;
  const char *tmrate = NULL;
  uerr_t err;
  struct_stat st;

   
  bool warc_enabled = (opt.warc_filename != NULL);
  FILE *warc_tmp = NULL;
  ip_address *warc_ip = NULL;
  wgint last_expected_bytes = 0;

   
  if ((f == NULL) && (con->target))
    {
       
      locf = con->target;
    }
  else
    {
       
      xfree (con->target);
      con->target = url_file_name (u, NULL);
      if (!opt.output_document)
        locf = con->target;
      else
        locf = opt.output_document;
    }

   

   
  if (opt.noclobber && !opt.output_document && file_exists_p (con->target)
      && !((con->cmd & DO_LIST) && !(con->cmd & DO_RETR)))
    {
      logprintf (LOG_VERBOSE,
                 _("File %s already there; not retrieving.\n"), quote (con->target));
       
      return RETROK;
    }

   
  remove_link (con->target);

  count = 0;

  if (con->st & ON_YOUR_OWN)
    con->st = ON_YOUR_OWN;

  orig_lp = con->cmd & LEAVE_PENDING ? 1 : 0;

   
  do
    {
       
      ++count;
      sleep_between_retrievals (count);
      if (con->st & ON_YOUR_OWN)
        {
          con->cmd = 0;
          con->cmd |= (DO_RETR | LEAVE_PENDING);
          if (con->csock != -1)
            con->cmd &= ~ (DO_LOGIN | DO_CWD);
          else
            con->cmd |= (DO_LOGIN | DO_CWD);
        }
      else  
        {
          if (con->csock != -1)
            con->cmd &= ~DO_LOGIN;
          else
            con->cmd |= DO_LOGIN;
          if (con->st & DONE_CWD)
            con->cmd &= ~DO_CWD;
          else
            con->cmd |= DO_CWD;
        }

       
      if (warc_enabled && (con->cmd & DO_RETR) && warc_tmp == NULL)
        {
          warc_tmp = warc_tempfile ();
          if (warc_tmp == NULL)
            return WARC_TMP_FOPENERR;

          if (!con->proxy && con->csock != -1)
            {
              warc_ip = (ip_address *) alloca (sizeof (ip_address));
              socket_ip_address (con->csock, warc_ip, ENDPOINT_PEER);
            }
        }

       
      if (con->cmd & DO_LIST)
        restval = 0;
      else if (force_full_retrieve)
        restval = 0;
      else if (opt.start_pos >= 0)
        restval = opt.start_pos;
      else if (opt.always_rest
          && stat (locf, &st) == 0
          && S_ISREG (st.st_mode))
         
        restval = st.st_size;
      else if (count > 1)
        restval = qtyread;           
      else
        restval = 0;

       
      tms = datetime_str (time (NULL));
       
      if (opt.verbose)
        {
          char *hurl = url_string (u, URL_AUTH_HIDE_PASSWD);
          char tmp[256];
          strcpy (tmp, "        ");
          if (count > 1)
            sprintf (tmp, _("(try:%2d)"), count);
          logprintf (LOG_VERBOSE, "--%s--  %s\n  %s => %s\n",
                     tms, hurl, tmp, quote (locf));
#ifdef WINDOWS
          ws_changetitle (hurl);
#endif
          xfree (hurl);
        }
       
      if (f && f->type != FT_SYMLINK)
        len = f->size;
      else
        len = 0;

       
      err = getftp (u, len, &qtyread, restval, con, count, &last_expected_bytes,
                    warc_tmp);

      if (con->csock == -1)
        con->st &= ~DONE_CWD;
      else
        con->st |= DONE_CWD;

      switch (err)
        {
        case HOSTERR: case CONIMPOSSIBLE: case FWRITEERR: case FOPENERR:
        case FTPNSFOD: case FTPLOGINC: case FTPNOPASV: case CONTNOTSUPPORTED:
        case UNLINKERR: case WARC_TMP_FWRITEERR:
           
          if (warc_tmp != NULL)
              fclose (warc_tmp);
          return err;
        case CONSOCKERR: case CONERROR: case FTPSRVERR: case FTPRERR:
        case WRITEFAILED: case FTPUNKNOWNTYPE: case FTPSYSERR:
        case FTPPORTERR: case FTPLOGREFUSED: case FTPINVPASV:
        case FOPEN_EXCL_ERR:
          printwhat (count, opt.ntry);
           
          if (err == FOPEN_EXCL_ERR)
            {
               
              xfree (con->target);
              con->target = url_file_name (u, NULL);
              locf = con->target;
            }
          continue;
        case FTPRETRINT:
           
          if (!f || qtyread != f->size)
            {
              printwhat (count, opt.ntry);
              continue;
            }
          break;
        case RETRFINISHED:
           
          break;
        default:
           
          abort ();
        }
      tms = datetime_str (time (NULL));
      if (!opt.spider)
        tmrate = retr_rate (qtyread - restval, con->dltime);

       
      downloaded_file (FILE_DOWNLOADED_NORMALLY, locf);

      if (con->st & ON_YOUR_OWN)
        {
          fd_close (con->csock);
          con->csock = -1;
        }
      if (!opt.spider)
        {
          bool write_to_stdout = (opt.output_document && HYPHENP (opt.output_document));

          logprintf (LOG_VERBOSE,
                     write_to_stdout
                     ? _("%s (%s) - written to stdout %s[%s]\n\n")
                     : _("%s (%s) - %s saved [%s]\n\n"),
                     tms, tmrate,
                     write_to_stdout ? "" : quote (locf),
                     number_to_static_string (qtyread));
        }
      if (!opt.verbose && !opt.quiet)
        {
           
          char *hurl = url_string (u, URL_AUTH_HIDE_PASSWD);
          logprintf (LOG_NONVERBOSE, "%s URL: %s [%s] -> \"%s\" [%d]\n",
                     tms, hurl, number_to_static_string (qtyread), locf, count);
          xfree (hurl);
        }

      if (warc_enabled && (con->cmd & DO_RETR))
        {
           
          bool warc_res;

          warc_res = warc_write_resource_record (NULL, u->url, NULL, NULL,
                                                  warc_ip, NULL, warc_tmp, -1);

          if (! warc_res)
            return WARC_ERR;

           
          warc_tmp = NULL;
        }

      if (con->cmd & DO_LIST)
         
        {
          if (!opt.remove_listing)
             
            {
              total_downloaded_bytes += qtyread;
              numurls++;
            }

           
        }
      else if (!opt.spider)
         
        {
           
          total_downloaded_bytes += qtyread;
          numurls++;

          if (opt.delete_after && !input_file_url (opt.input_filename))
            {
              DEBUGP (("\
Removing file due to --delete-after in ftp_loop_internal():\n"));
              logprintf (LOG_VERBOSE, _("Removing %s.\n"), locf);
              if (unlink (locf))
                logprintf (LOG_NOTQUIET, "unlink: %s\n", strerror (errno));
            }
        }

       
      if (orig_lp)
        con->cmd |= LEAVE_PENDING;
      else
        con->cmd &= ~LEAVE_PENDING;

      if (local_file)
        *local_file = xstrdup (locf);

      if (warc_tmp != NULL)
        fclose (warc_tmp);

      return RETROK;
    } while (!opt.ntry || (count < opt.ntry));

  if (con->csock != -1 && (con->st & ON_YOUR_OWN))
    {
      fd_close (con->csock);
      con->csock = -1;
    }

  if (warc_tmp != NULL)
    fclose (warc_tmp);

  return TRYLIMEXC;
}
