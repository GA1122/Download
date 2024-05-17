wiki_print_data_as_html(HttpResponse *res, char *raw_page_data)
{
  char *p = raw_page_data;	     
  char *q = NULL, *link = NULL;  
  char *line = NULL;
  int   line_len;
  int   i, j, skip_chars;

   
  int bold_on      = 0;
  int italic_on    = 0;
  int underline_on = 0;
  int strikethrough_on = 0;
  int open_para    = 0;
  int pre_on       = 0;
  int table_on     = 0;

#define ULIST 0
#define OLIST 1
#define NUM_LIST_TYPES 2

  struct { char ident; int  depth; char *tag; } listtypes[] = {
    { '*', 0, "ul" },
    { '#', 0, "ol" }
  };


  q = p;   

  while ( (line = get_line_from_string(&q, &line_len)) )
    {
      int   header_level = 0; 
      char *line_start   = line;
      int   skip_to_content = 0;
       

      if (pre_on && !isspace(*line) && *line != '\0')
	{
	   
	  http_response_printf(res, "\n</pre>\n") ;
	  pre_on = 0;
	}

       
      for (i=0; i<NUM_LIST_TYPES; i++)
	{

	   
	  if ( *line == listtypes[i].ident
	       && ( *(line+1) == listtypes[i].ident || isspace(*(line+1)) ) ) 
	    {                     	
	      int item_depth = 0;

	      if (listtypes[!i].depth)
		{
		  for (j=0; j<listtypes[!i].depth; j++)
		    http_response_printf(res, "</%s>\n", listtypes[!i].tag);
		  listtypes[!i].depth = 0;
		}

	      while ( *line == listtypes[i].ident ) { line++; item_depth++; }
	  
	      if (item_depth < listtypes[i].depth)
		{
		  for (j = 0; j < (listtypes[i].depth - item_depth); j++)
		    http_response_printf(res, "</%s>\n", listtypes[i].tag);
		}
	      else
		{
		  for (j = 0; j < (item_depth - listtypes[i].depth); j++)
		    http_response_printf(res, "<%s>\n", listtypes[i].tag);
		}
	      
	      http_response_printf(res, "<li>");
	      
	      listtypes[i].depth = item_depth;
	      
	      skip_to_content = 1;
	    }
	  else if (listtypes[i].depth && !listtypes[!i].depth) 
	    {
	       

	      for (j=0; j<listtypes[i].depth; j++)
		http_response_printf(res, "</%s>\n", listtypes[i].tag);
	      listtypes[i].depth = 0;
	    }
	}

      if (skip_to_content)
	goto line_content;  

       

      if (*line == '|')
        {
	  if (table_on==0)
	    http_response_printf(res, "<table class='wikitable' cellspacing='0' cellpadding='4'>\n");
	  line++;

	    http_response_printf(res, "<tr><td>");

	  table_on = 1;
	  goto line_content;
        }
      else
        {
	  if(table_on)
	    {
	      http_response_printf(res, "</table>\n");
	      table_on = 0;
	    }
        }

       

      if ( (isspace(*line) || *line == '\0'))
	{
	  int n_spaces = 0;

	  while ( isspace(*line) ) { line++; n_spaces++; }

	  if (*line == '\0')   
	    {
	      if (pre_on)
		{
		  http_response_printf(res, "\n") ;
		  continue;
		}
	      else if (open_para)
		{
		  http_response_printf(res, "\n</p><p>\n") ;
		}
	      else
		{
		  http_response_printf(res, "\n<p>\n") ;
		  open_para = 1;
		}
	    }
	  else  
	    {
	      if (!pre_on)
		http_response_printf(res, "<pre>\n") ;
	      pre_on = 1;
	      line = line - ( n_spaces - 1 );  
	      http_response_printf(res, "%s\n", line);
	      continue;
	    }
	}
      else if ( *line == '=' )
	{
	  while (*line == '=')
	    { header_level++; line++; }

	  http_response_printf(res, "<h%d>", header_level);
	  p = line;
	}
      else if ( *line == '-' && *(line+1) == '-' )
	{
	   
	  http_response_printf(res, "<hr/>\n");
	  while ( *line == '-' ) line++;
	}

    line_content:

       

      p = line;

      while ( *line != '\0' )
	{
	  if ( *line == '!' && !isspace(*(line+1))) 
	    {                	 
	      *line = '\0';
	      http_response_printf(res, "%s", p);
	      p = ++line;

	      while (*line != '\0' && !isspace(*line)) line++;
	      if (*line == '\0')
		continue;
	    }
	  else if ((link = check_for_link(line, &skip_chars)) != NULL)
	    {
	      http_response_printf(res, "%s", p);
	      http_response_printf(res, "%s", link); 

	      line += skip_chars;
	      p = line;

	      continue;

	    }
	   
	  else if (*line == '*')
	    {
	       
	      if (line_start != line 
		  && !is_wiki_format_char_or_space(*(line-1)) 
		  && !bold_on)
		{ line++; continue; }

	      if ((isspace(*(line+1)) && !bold_on))
		{ line++; continue; }

		 
		*line = '\0';
		http_response_printf(res, "%s%s\n", p, bold_on ? "</b>" : "<b>");
		bold_on ^= 1;  
		p = line+1;

	    }
	  else if (*line == '_' )
	    {
	      if (line_start != line 
		  && !is_wiki_format_char_or_space(*(line-1)) 
		  && !underline_on)
		{ line++; continue; }

	      if (isspace(*(line+1)) && !underline_on)
		{ line++; continue; }
	       
	      *line = '\0';
	      http_response_printf(res, "%s%s\n", p, underline_on ? "</u>" : "<u>"); 
	      underline_on ^= 1;  
	      p = line+1;
	    }
	  else if (*line == '-')
	    {
	      if (line_start != line 
		  && !is_wiki_format_char_or_space(*(line-1)) 
		  && !strikethrough_on)
		{ line++; continue; }

	      if (isspace(*(line+1)) && !strikethrough_on)
		{ line++; continue; }
	       
	       
	      *line = '\0';
	      http_response_printf(res, "%s%s\n", p, strikethrough_on ? "</del>" : "<del>"); 
	      strikethrough_on ^= 1;  
	      p = line+1; 
	      

	    }
	  else if (*line == '/' )
	    {
	      if (line_start != line 
		  && !is_wiki_format_char_or_space(*(line-1)) 
		  && !italic_on)
		{ line++; continue; }

	      if (isspace(*(line+1)) && !italic_on)
		{ line++; continue; }

	       
	      if (line_start != line && isspace(*(line-1)) && !italic_on)
		{ 
		  char *tmp   = line+1;
		  int slashes = 0;

		   
		  while (*tmp != '\0' && !isspace(*tmp))
		    { 
		      if (*tmp == '/') slashes++;
		      tmp++;
		    }

		  if (slashes > 1 || (slashes == 1 && *(tmp-1) != '/')) 
		    { line = tmp; continue; }
		}

	      if (*(line+1) == '/')
		line++; 	 
	      else
		{
		   
		  *line = '\0';
		  http_response_printf(res, "%s%s\n", p, italic_on ? "</i>" : "<i>"); 
		  italic_on ^= 1;  
		  p = line+1; 
		}
	    }
	  else if (*line == '|' && table_on)  
	    {
	      *line = '\0';
	      http_response_printf(res, "%s", p);
	      http_response_printf(res, "</td><td>\n");
	      p = line+1;
	    }

	  line++;

	}  

      if (*p != '\0') 			 
	http_response_printf(res, "%s", p);

       


      if (listtypes[ULIST].depth)
	http_response_printf(res, "</li>");

      if (listtypes[OLIST].depth)
	http_response_printf(res, "</li>");

      if (table_on)
	http_response_printf(res, "</td></tr>\n");

      if (header_level)
	http_response_printf(res, "</h%d>\n", header_level);  
      else
	http_response_printf(res, "\n");


    }  

   

  if (pre_on)
    http_response_printf(res, "</pre>\n");
  
   
  for (i=0; i<listtypes[ULIST].depth; i++)
    http_response_printf(res, "</ul>\n");

  for (i=0; i<listtypes[OLIST].depth; i++)
    http_response_printf(res, "</ol>\n");
  
   
  if (open_para)
    http_response_printf(res, "</p>\n");

     
    if (table_on)
     http_response_printf(res, "</table>\n");

}
