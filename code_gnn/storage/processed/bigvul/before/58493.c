wiki_init(void)
{
  char datadir[512] = { 0 };
  struct stat st;

  if (getenv("DIDIWIKIHOME"))
    {
      snprintf(datadir, 512, getenv("DIDIWIKIHOME"));
    }
  else
    {
      if (getenv("HOME") == NULL)
	{
	  fprintf(stderr, "Unable to get home directory, is HOME set?\n");
	  exit(1);
	}

      snprintf(datadir, 512, "%s/.didiwiki", getenv("HOME"));
    }  
     
   
  if (stat(datadir, &st) != 0 )
    {
      if (mkdir(datadir, 0755) == -1)
	{
	  fprintf(stderr, "Unable to create '%s', giving up.\n", datadir);
	  exit(1);
	}
    }

  chdir(datadir);

   

  if (access("WikiHelp", R_OK) != 0) 
    file_write("WikiHelp", HELPTEXT);

  if (access("WikiHome", R_OK) != 0) 
    file_write("WikiHome", HOMETEXT);

   

  if (access("styles.css", R_OK) == 0) 
    CssData = file_read("styles.css");
  
  return 1;
}
