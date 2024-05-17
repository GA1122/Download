void AppendForcedMostVisitedURL(const GURL& url,
                                double last_forced_time,
                                std::vector<MostVisitedURL>* list) {
  MostVisitedURL mv;
  mv.url = url;
  mv.last_forced_time = base::Time::FromJsTime(last_forced_time);
  mv.redirects.push_back(url);
  list->push_back(mv);
}
