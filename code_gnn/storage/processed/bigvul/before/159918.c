void AppendMostVisitedURLWithRedirect(const GURL& redirect_source,
                                      const GURL& redirect_dest,
                                      std::vector<MostVisitedURL>* list) {
  MostVisitedURL mv;
  mv.url = redirect_dest;
  mv.redirects.push_back(redirect_source);
  mv.redirects.push_back(redirect_dest);
  list->push_back(mv);
}
