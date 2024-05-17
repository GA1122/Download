wiki_show_search_results_page(HttpResponse *res, char *expr)
{
  WikiPageList **pages = NULL;
  int            n_pages, i;

  if (expr == NULL || strlen(expr) == 0)
    {
      wiki_show_header(res, "Search", FALSE);
      http_response_printf(res, "No Search Terms supplied");
      wiki_show_footer(res);
      http_response_send(res);
      exit(0);
    }

  pages = wiki_get_pages(&n_pages, expr);

  if (pages)
    {
      for (i=0; i<n_pages; i++)
	if (!strcmp(pages[i]->name, expr))  
	  wiki_redirect(res, pages[i]->name);

      wiki_show_header(res, "Search", FALSE);

      for (i=0; i<n_pages; i++)
	{
	  http_response_printf(res, "<a href='%s'>%s</a><br />\n", 
			       pages[i]->name, 
			       pages[i]->name);
	}
    }
  else 
    {
      wiki_show_header(res, "Search", FALSE);
      http_response_printf(res, "No matches");
    }

  wiki_show_footer(res);
  http_response_send(res);

  exit(0);
}