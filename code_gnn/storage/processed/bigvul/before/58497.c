wiki_show_changes_page_rss(HttpResponse *res)
{
  WikiPageList **pages = NULL;
  int            n_pages, i;
   

  pages = wiki_get_pages(&n_pages, NULL);

  http_response_printf(res, "<?xml version=\"1.0\"encoding=\"ISO-8859-1\"?>\n"
                            "<rss version=\"2.0\">\n"
		            "<channel><title>DidiWiki Changes feed</title>\n");

  for (i=0; i<n_pages; i++)
    {
      struct tm   *pTm;
      char         datebuf[64];

      pTm = localtime(&pages[i]->mtime);
      strftime(datebuf, sizeof(datebuf), "%Y-%m-%d %H:%M", pTm);

      http_response_printf(res, 
			   "<item><title>%s</title>"
			   "<link>%s%s</link><description>"
			   "Modified %s\n",
                           pages[i]->name,
			   getenv("DIDIWIKI_URL_PREFIX") ? getenv("DIDIWIKI_URL_PREFIX") : "", 
                           pages[i]->name,
			   datebuf);

       
      http_response_printf(res, "</description></item>\n");
    }

  http_response_printf(res, "</channel>\n</rss>");

  http_response_send(res);
  exit(0);
}
