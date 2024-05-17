url_file_name (const struct url *u, char *replaced_filename)
{
  struct growable fnres;         
  struct growable temp_fnres;

  const char *u_file;
  char *fname, *unique, *fname_len_check;
  const char *index_filename = "index.html";  
  size_t max_length;

  fnres.base = NULL;
  fnres.size = 0;
  fnres.tail = 0;

  temp_fnres.base = NULL;
  temp_fnres.size = 0;
  temp_fnres.tail = 0;

   
  if (opt.default_page)
    index_filename = opt.default_page;


   
  if (opt.dir_prefix)
    append_string (opt.dir_prefix, &fnres);

   
  if (opt.dirstruct)
    {
      if (opt.protocol_directories)
        {
          if (fnres.tail)
            append_char ('/', &fnres);
          append_string (supported_schemes[u->scheme].name, &fnres);
        }
      if (opt.add_hostdir)
        {
          if (fnres.tail)
            append_char ('/', &fnres);
          if (0 != strcmp (u->host, ".."))
            append_string (u->host, &fnres);
          else
             
            append_string ("%2E%2E", &fnres);
          if (u->port != scheme_default_port (u->scheme))
            {
              char portstr[24];
              number_to_string (portstr, u->port);
              append_char (FN_PORT_SEP, &fnres);
              append_string (portstr, &fnres);
            }
        }

      append_dir_structure (u, &fnres);
    }

  if (!replaced_filename)
    {
       
      u_file = *u->file ? u->file : index_filename;

       
      if (u->query)
        fname_len_check = concat_strings (u_file, FN_QUERY_SEP_STR, u->query, NULL);
      else
        fname_len_check = strdupdelim (u_file, u_file + strlen (u_file));

       
      fname_len_check = convert_fname (fname_len_check);
    }
  else
    {
      u_file = replaced_filename;
      fname_len_check = strdupdelim (u_file, u_file + strlen (u_file));
    }

  append_uri_pathel (fname_len_check,
    fname_len_check + strlen (fname_len_check), false, &temp_fnres);

   
  append_char ('\0', &temp_fnres);

   
#ifdef WINDOWS
  if (MAX_PATH > (fnres.tail + CHOMP_BUFFER + 2))
    {
      max_length = MAX_PATH - (fnres.tail + CHOMP_BUFFER + 2);
       
      if (max_length > 255)
        {
          max_length = 255;
        }
    }
  else
    {
      max_length = 0;
    }
#else
  max_length = get_max_length (fnres.base, fnres.tail, _PC_NAME_MAX) - CHOMP_BUFFER;
#endif
  if (max_length > 0 && strlen (temp_fnres.base) > max_length)
    {
      logprintf (LOG_NOTQUIET, "The name is too long, %lu chars total.\n",
          (unsigned long) strlen (temp_fnres.base));
      logprintf (LOG_NOTQUIET, "Trying to shorten...\n");

       
      temp_fnres.base[max_length] = '\0';

      logprintf (LOG_NOTQUIET, "New name is %s.\n", temp_fnres.base);
    }

  xfree (fname_len_check);

   
  if (fnres.tail)
    append_char ('/', &fnres);
  append_string (temp_fnres.base, &fnres);

  fname = fnres.base;

   
   

  xfree (temp_fnres.base);

   

  if (ALLOW_CLOBBER
      && !(file_exists_p (fname) && !file_non_directory_p (fname)))
    {
      unique = fname;
    }
  else
    {
      unique = unique_name (fname, true);
      if (unique != fname)
        xfree (fname);
    }

 
#ifdef __VMS
  {
    char *unique2;

    unique2 = ods_conform( unique);
    if (unique2 != unique)
      {
        xfree (unique);
        unique = unique2;
      }
  }
#endif  

  return unique;
}
