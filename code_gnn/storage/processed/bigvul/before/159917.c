void AppendMostVisitedURL(const GURL& url, std::vector<MostVisitedURL>* list) {
  MostVisitedURL mv;
  mv.url = url;
  mv.redirects.push_back(url);
  list->push_back(mv);
}
