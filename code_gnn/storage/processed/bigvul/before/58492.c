wiki_get_pages(int  *n_pages, char *expr)
{
  WikiPageList  **pages;
  struct dirent **namelist;
  int             n, i = 0;
  struct stat     st;

  n = scandir(".", &namelist, 0, (void *)changes_compar);
  
  pages = malloc(sizeof(WikiPageList*)*n);

  while(n--) 
    {
      if ((namelist[n]->d_name)[0] == '.' 
	  || !strcmp(namelist[n]->d_name, "styles.css"))
	goto cleanup;

      if (expr != NULL) 
	{ 			 
	  char *data = NULL;
	  if ((data = file_read(namelist[n]->d_name)) != NULL)
	    if (strstr(data, expr) == NULL)
	      if (strcmp(namelist[n]->d_name, expr) != 0) 
		goto cleanup; 
	}


      stat(namelist[n]->d_name, &st);

       
      if (S_ISREG(st.st_mode) && access(namelist[n]->d_name, R_OK) == 0)
	{
	  pages[i]        = malloc(sizeof(WikiPageList));
	  pages[i]->name  = strdup (namelist[n]->d_name);
	  pages[i]->mtime = st.st_mtime;

	  i++;
	}

    cleanup:
      free(namelist[n]);
    }

  *n_pages = i;

  free(namelist);

  if (i==0) return NULL;

  return pages;
}