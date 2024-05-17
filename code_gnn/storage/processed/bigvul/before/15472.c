check_file_output (const struct url *u, struct http_stat *hs,
                   struct response *resp, char *hdrval, size_t hdrsize)
{
   
  if (!hs->local_file)
    {
      char *local_file = NULL;

       
      if (!opt.content_disposition
          || !resp_header_copy (resp, "Content-Disposition",
                                hdrval, hdrsize)
          || !parse_content_disposition (hdrval, &local_file))
        {
           
          hs->local_file = url_file_name (u, NULL);
        }
      else
        {
          DEBUGP (("Parsed filename from Content-Disposition: %s\n",
                  local_file));
          hs->local_file = url_file_name (u, local_file);
        }

      xfree (local_file);
    }

  hs->temporary = opt.delete_after || opt.spider || !acceptable (hs->local_file);
  if (hs->temporary)
    {
      char *tmp = aprintf ("%s.tmp", hs->local_file);
      xfree (hs->local_file);
      hs->local_file = tmp;
    }

   
  if (!hs->existence_checked && file_exists_p (hs->local_file, NULL))
    {
      if (opt.noclobber && !opt.output_document)
        {
           
          return RETRUNNEEDED;
        }
      else if (!ALLOW_CLOBBER)
        {
          char *unique = unique_name (hs->local_file, true);
          if (unique != hs->local_file)
            xfree (hs->local_file);
          hs->local_file = unique;
        }
    }
  hs->existence_checked = true;

   
  if (opt.timestamping && !hs->timestamp_checked)
    {
      uerr_t timestamp_err = set_file_timestamp (hs);
      if (timestamp_err != RETROK)
        return timestamp_err;
    }
  return RETROK;
}
