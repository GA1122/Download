install_cupsd_conf(cupsd_client_t *con)	 
{
  char		filename[1024];		 
  cups_file_t	*in,			 
		*out;			 
  char		buffer[16384];		 
  ssize_t	bytes;			 


  

  if ((in = cupsFileOpen(con->filename, "rb")) == NULL)
  {
    cupsdLogClient(con, CUPSD_LOG_ERROR, "Unable to open request file \"%s\": %s",
                    con->filename, strerror(errno));
    goto server_error;
  }

  

  if ((out = cupsdCreateConfFile(ConfigurationFile, ConfigFilePerm)) == NULL)
  {
    cupsFileClose(in);
    goto server_error;
  }

  cupsdLogClient(con, CUPSD_LOG_INFO, "Installing config file \"%s\"...",
                  ConfigurationFile);

  

  while ((bytes = cupsFileRead(in, buffer, sizeof(buffer))) > 0)
    if (cupsFileWrite(out, buffer, (size_t)bytes) < bytes)
    {
      cupsdLogClient(con, CUPSD_LOG_ERROR,
                      "Unable to copy to config file \"%s\": %s",
        	      ConfigurationFile, strerror(errno));

      cupsFileClose(in);
      cupsFileClose(out);

      snprintf(filename, sizeof(filename), "%s.N", ConfigurationFile);
      cupsdUnlinkOrRemoveFile(filename);

      goto server_error;
    }

  

  cupsFileClose(in);

  if (cupsdCloseCreatedConfFile(out, ConfigurationFile))
    goto server_error;

  

  cupsdUnlinkOrRemoveFile(con->filename);
  cupsdClearString(&con->filename);

  

  NeedReload = RELOAD_CUPSD;
  ReloadTime = time(NULL);

  

  return (HTTP_STATUS_CREATED);

  

  server_error:

  cupsdUnlinkOrRemoveFile(con->filename);
  cupsdClearString(&con->filename);

  return (HTTP_STATUS_SERVER_ERROR);
}
