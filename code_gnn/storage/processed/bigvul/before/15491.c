metalink_from_http (const struct response *resp, const struct http_stat *hs,
                    const struct url *u)
{
  metalink_t *metalink = NULL;
  metalink_file_t *mfile = xnew0 (metalink_file_t);
  const char *val_beg, *val_end;
  int res_count = 0, meta_count = 0, hash_count = 0, sig_count = 0, i;

  DEBUGP (("Checking for Metalink in HTTP response\n"));

   
  if (hs->local_file)
    mfile->name = xstrdup (hs->local_file);
  else
    mfile->name = url_file_name (u, NULL);

   
  mfile->checksums = xnew0 (metalink_checksum_t *);
  mfile->resources = xnew0 (metalink_resource_t *);
  mfile->metaurls = xnew0 (metalink_metaurl_t *);

   
  if (resp_header_locate (resp, "Content-Type", 0, &val_beg, &val_end) != -1)
    {
      metalink_metaurl_t murl = {0};

      const char *type_beg, *type_end;
      char *typestr = NULL;
      char *namestr = NULL;
      size_t type_len;

      DEBUGP (("Processing Content-Type header...\n"));

       
      type_beg = val_beg;
      while (type_beg < val_end && c_isspace (*type_beg))
        type_beg++;

       
      type_end = type_beg + 1;
      while (type_end < val_end &&
             *type_end != ';' &&
             *type_end != ' ' &&
             *type_end != '\r' &&
             *type_end != '\n')
        type_end++;

      if (type_beg >= val_end || type_end > val_end)
        {
          DEBUGP (("Invalid Content-Type header. Ignoring.\n"));
          goto skip_content_type;
        }

      type_len = type_end - type_beg;
      typestr = xstrndup (type_beg, type_len);

      DEBUGP (("Content-Type: %s\n", typestr));

      if (strcmp (typestr, "application/metalink4+xml"))
        {
          xfree (typestr);
          goto skip_content_type;
        }

       
      murl.priority = DEFAULT_PRI;

      murl.mediatype = typestr;
      typestr = NULL;

      if (opt.content_disposition
          && resp_header_locate (resp, "Content-Disposition", 0, &val_beg, &val_end) != -1)
        {
          find_key_value (val_beg, val_end, "filename", &namestr);
          murl.name = namestr;
          namestr = NULL;
        }

      murl.url = xstrdup (u->url);

      DEBUGP (("URL=%s\n", murl.url));
      DEBUGP (("MEDIATYPE=%s\n", murl.mediatype));
      DEBUGP (("NAME=%s\n", murl.name ? murl.name : ""));
      DEBUGP (("PRIORITY=%d\n", murl.priority));

       
      mfile->metaurls = xrealloc (mfile->metaurls,
                                  sizeof (metalink_metaurl_t *) * (meta_count + 2));
      mfile->metaurls[meta_count] = xnew0 (metalink_metaurl_t);
      *mfile->metaurls[meta_count] = murl;
      meta_count++;
    }
skip_content_type:

   
  for (i = 0;
       (i = resp_header_locate (resp, "Link", i, &val_beg, &val_end)) != -1;
       i++)
    {
      char *rel = NULL, *reltype = NULL;
      char *urlstr = NULL;
      const char *url_beg, *url_end, *attrs_beg;
      size_t url_len;

       

       
      url_beg = val_beg;
      while (url_beg < val_end - 1 && c_isspace (*url_beg))
        url_beg++;

       
       
      url_end = url_beg + 1;
      while (url_end < val_end - 1 && *url_end != '>')
        url_end++;

      if (url_beg >= val_end || url_end >= val_end ||
          *url_beg != '<' || *url_end != '>')
        {
          DEBUGP (("This is not a valid Link header. Ignoring.\n"));
          continue;
        }

       
      url_beg++;
      url_len = url_end - url_beg;

       
      attrs_beg = url_end + 1;

       
      if (!find_key_value (attrs_beg, val_end, "rel", &rel))
        {
          DEBUGP (("No rel value in Link header, skipping.\n"));
          continue;
        }

      urlstr = xstrndup (url_beg, url_len);
      DEBUGP (("URL=%s\n", urlstr));
      DEBUGP (("rel=%s\n", rel));

      if (!strcmp (rel, "describedby"))
        find_key_value (attrs_beg, val_end, "type", &reltype);

       
      if (sig_count == 0 &&
          reltype && !strcmp (reltype, "application/pgp-signature"))
        {
           
          FILE *_output_stream = output_stream;
          bool _output_stream_regular = output_stream_regular;

          output_stream = tmpfile ();
          if (output_stream)
            {
              struct iri *iri = iri_new ();
              struct url *url;
              int url_err;

              set_uri_encoding (iri, opt.locale, true);
              url = url_parse (urlstr, &url_err, iri, false);

              if (!url)
                {
                  char *error = url_error (urlstr, url_err);
                  logprintf (LOG_NOTQUIET, _("When downloading signature:\n"
                                             "%s: %s.\n"), urlstr, error);
                  xfree (error);
                  iri_free (iri);
                }
              else
                {
                   
                  bool _metalink_http = opt.metalink_over_http;
                  uerr_t retr_err;

                  opt.metalink_over_http = false;
                  retr_err = retrieve_url (url, urlstr, NULL, NULL,
                                           NULL, NULL, false, iri, false);
                  opt.metalink_over_http = _metalink_http;

                  url_free (url);
                  iri_free (iri);

                  if (retr_err == RETROK)
                    {
                       
                      metalink_signature_t msig;
                      size_t siglen;

                      fseek (output_stream, 0, SEEK_END);
                      siglen = ftell (output_stream);
                      fseek (output_stream, 0, SEEK_SET);

                      DEBUGP (("siglen=%lu\n", siglen));

                      msig.signature = xmalloc (siglen + 1);
                      if (fread (msig.signature, siglen, 1, output_stream) != 1)
                        {
                          logputs (LOG_NOTQUIET,
                                   _("Unable to read signature content from "
                                     "temporary file. Skipping.\n"));
                          xfree (msig.signature);
                        }
                      else
                        {
                          msig.signature[siglen] = '\0';  
                          msig.mediatype = xstrdup ("application/pgp-signature");

                          DEBUGP (("Signature (%s):\n%s\n",
                                   msig.mediatype, msig.signature));

                          mfile->signature = xnew (metalink_signature_t);
                          *mfile->signature = msig;

                          sig_count++;
                        }
                    }
                }
              fclose (output_stream);
            }
          else
            {
              logputs (LOG_NOTQUIET, _("Could not create temporary file. "
                                       "Skipping signature download.\n"));
            }
          output_stream_regular = _output_stream_regular;
          output_stream = _output_stream;
        }  

         
      else if (!strcmp (rel, "duplicate"))
        {
          metalink_resource_t mres = {0};
          char *pristr;

           
          mres.priority = DEFAULT_PRI;
          if (find_key_value (url_end, val_end, "pri", &pristr))
            {
              long pri;
              char *end_pristr;
               
              pri = strtol (pristr, &end_pristr, 10);
              if (end_pristr != pristr + strlen (pristr) ||
                  !VALID_PRI_RANGE (pri))
                {
                   
                  logprintf (LOG_NOTQUIET,
                             _("Invalid pri value. Assuming %d.\n"),
                             DEFAULT_PRI);
                }
              else
                mres.priority = pri;
              xfree (pristr);
            }

          switch (url_scheme (urlstr))
            {
            case SCHEME_HTTP:
              mres.type = xstrdup ("http");
              break;
#ifdef HAVE_SSL
            case SCHEME_HTTPS:
              mres.type = xstrdup ("https");
              break;
            case SCHEME_FTPS:
              mres.type = xstrdup ("ftps");
              break;
#endif
            case SCHEME_FTP:
              mres.type = xstrdup ("ftp");
              break;
            default:
              DEBUGP (("Unsupported url scheme in %s. Skipping resource.\n", urlstr));
            }

          if (mres.type)
            {
              DEBUGP (("TYPE=%s\n", mres.type));

               

              find_key_value (url_end, val_end, "geo", &mres.location);

              mres.url = urlstr;
              urlstr = NULL;

              mres.preference = 0;
              if (has_key (url_end, val_end, "pref"))
                {
                  DEBUGP (("This resource has preference\n"));
                  mres.preference = 1;
                }

               
              mfile->resources = xrealloc (mfile->resources,
                                           sizeof (metalink_resource_t *) * (res_count + 2));
              mfile->resources[res_count] = xnew0 (metalink_resource_t);
              *mfile->resources[res_count] = mres;
              res_count++;
            }
        }  

       
      else if (reltype && !strcmp (reltype, "application/metalink4+xml"))
        {
          metalink_metaurl_t murl = {0};
          char *pristr;

           
          murl.priority = DEFAULT_PRI;
          if (find_key_value (url_end, val_end, "pri", &pristr))
            {
              long pri;
              char *end_pristr;
               
              pri = strtol (pristr, &end_pristr, 10);
              if (end_pristr != pristr + strlen (pristr) ||
                  !VALID_PRI_RANGE (pri))
                {
                   
                  logprintf (LOG_NOTQUIET,
                             _("Invalid pri value. Assuming %d.\n"),
                             DEFAULT_PRI);
                }
              else
                murl.priority = pri;
              xfree (pristr);
            }

          murl.mediatype = xstrdup (reltype);

          DEBUGP (("MEDIATYPE=%s\n", murl.mediatype));

           

          find_key_value (url_end, val_end, "name", &murl.name);

          murl.url = urlstr;
          urlstr = NULL;

           
          mfile->metaurls = xrealloc (mfile->metaurls,
                                       sizeof (metalink_metaurl_t *) * (meta_count + 2));
          mfile->metaurls[meta_count] = xnew0 (metalink_metaurl_t);
          *mfile->metaurls[meta_count] = murl;
          meta_count++;
        }  
      else
        DEBUGP (("This link header was not used for Metalink\n"));

      xfree (urlstr);
      xfree (reltype);
      xfree (rel);
    }  

   
  mfile->resources[res_count] = 0;
  mfile->metaurls[meta_count] = 0;

  if (res_count == 0 && meta_count == 0)
    {
      DEBUGP (("No valid metalink references found.\n"));
      goto fail;
    }

   
  for (i = 0;
       (i = resp_header_locate (resp, "Digest", i, &val_beg, &val_end)) != -1;
       i++)
    {
      const char *dig_pos;
      char *dig_type, *dig_hash;

       
      for (dig_pos = val_beg;
           (dig_pos = find_key_values (dig_pos, val_end, &dig_type, &dig_hash));
           dig_pos++)
        {
           
          const size_t dig_hash_str_len = strlen (dig_hash);
          char *bin_hash = alloca (dig_hash_str_len * 3 / 4 + 1);
          ssize_t hash_bin_len;

          hash_bin_len = wget_base64_decode (dig_hash, bin_hash, dig_hash_str_len * 3 / 4 + 1);

           
          if (hash_bin_len < 0)
            {
              xfree (dig_type);
              xfree (dig_hash);
              continue;
            }

           
          mfile->checksums =
                  xrealloc (mfile->checksums,
                            sizeof (metalink_checksum_t *) * (hash_count + 2));
          mfile->checksums[hash_count] = xnew (metalink_checksum_t);
          mfile->checksums[hash_count]->type = dig_type;

          mfile->checksums[hash_count]->hash = xmalloc ((size_t)hash_bin_len * 2 + 1);
          wg_hex_to_string (mfile->checksums[hash_count]->hash, bin_hash, (size_t)hash_bin_len);

          xfree (dig_hash);

          hash_count++;
        }
    }

   
  mfile->checksums[hash_count] = 0;

   
  if (res_count && hash_count == 0)
    {
      logputs (LOG_VERBOSE,
               _("Could not find acceptable digest for Metalink resources.\n"
                 "Ignoring them.\n"));
      goto fail;
    }

   
  stable_sort (mfile->resources, res_count, sizeof (metalink_resource_t *), metalink_res_cmp);
  stable_sort (mfile->metaurls, meta_count, sizeof (metalink_metaurl_t *), metalink_meta_cmp);

   
  for (i = 0; i < res_count; ++i)
    mfile->resources[i]->preference = 1000000 - mfile->resources[i]->priority;

  metalink = xnew0 (metalink_t);
  metalink->files = xmalloc (sizeof (metalink_file_t *) * 2);
  metalink->files[0] = mfile;
  metalink->files[1] = 0;
  metalink->origin = xstrdup (u->url);
  metalink->version = METALINK_VERSION_4;
   

  return metalink;

fail:
   
  if (metalink)
    metalink_delete (metalink);
  else
    metalink_file_delete (mfile);
  return NULL;
}
