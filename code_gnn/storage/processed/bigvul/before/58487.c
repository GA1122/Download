check_for_link(char *line, int *skip_chars)
{
  char *start =  line;
  char *p     =  line;
  char *url   =  NULL;
  char *title =  NULL;
  char *result = NULL;
  int   found = 0;

  if (*p == '[') 		 
    {
       
      url = start+1; *p = '\0'; p++;
      while (  *p != ']' && *p != '\0' && !isspace(*p) ) p++;

      if (isspace(*p))
	{
	  *p = '\0';
	  title = ++p; 
	  while (  *p != ']' && *p != '\0' ) 
	    p++;
	}

      *p = '\0';
      p++;
    }                     
  else if (!strncasecmp(p, "http://", 7)
	   || !strncasecmp(p, "mailto://", 9)
	   || !strncasecmp(p, "file://", 7))
    {
      while ( *p != '\0' && !isspace(*p) ) p++;


      found = 1;
    }
  else if (isupper(*p))      	 
    {
      int num_upper_char = 1;
      p++;
      while ( *p != '\0' && isalnum(*p) )
	{
	  if (isupper(*p))
	    { found = 1; num_upper_char++; }
	  p++;
	}

      if (num_upper_char == (p-start))  
	return NULL;
    }

  if (found)   
    {
      url = malloc(sizeof(char) * ((p - start) + 2) );
      memset(url, 0, sizeof(char) * ((p - start) + 2));
      strncpy(url, start, p - start);
      *start = '\0';
    }

  if (url != NULL)
    {
      int len = strlen(url);

      *skip_chars = p - start;

       
      if (!strncmp(url+len-4, ".gif", 4) || !strncmp(url+len-4, ".png", 4) 
	  || !strncmp(url+len-4, ".jpg", 4) || !strncmp(url+len-5, ".jpeg", 5))
	{
	  if (title)
	    asprintf(&result, "<a href='%s'><img src='%s' border='0'></a>",
		     title, url);
	  else
	    asprintf(&result, "<img src='%s' border='0'>", url);
	}
      else
	{
	  char *extra_attr = "";

	  if (!strncasecmp(url, "http://", 7))
	    extra_attr = " title='WWW link' ";

	  if (title)
	    asprintf(&result,"<a %s href='%s'>%s</a>", extra_attr, url, title);
	  else
	    asprintf(&result, "<a %s href='%s'>%s</a>", extra_attr, url, url);
	}

      

      return result;
    }

  return NULL;
}