  void Navigate(const char* url, int page_id) {
    contents()->TestDidNavigate(
        contents()->render_view_host(), page_id, GURL(url),
         content::PAGE_TRANSITION_TYPED);
   }
