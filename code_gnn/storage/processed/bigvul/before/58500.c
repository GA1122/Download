wiki_show_footer(HttpResponse *res)
{
  http_response_printf(res, "%s", PAGEFOOTER);

  http_response_printf(res, 
		       "</body>\n"
		       "</html>\n"
 		       );
 }