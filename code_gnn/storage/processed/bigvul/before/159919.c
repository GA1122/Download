void AppendMostVisitedURLwithTitle(const GURL& url,
                                   const base::string16& title,
                                   std::vector<MostVisitedURL>* list) {
  MostVisitedURL mv;
  mv.url = url;
  mv.title = title;
  mv.redirects.push_back(url);
  list->push_back(mv);
}
