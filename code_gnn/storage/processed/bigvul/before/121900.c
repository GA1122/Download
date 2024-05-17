void ForceGoogleSafeSearch(net::URLRequest* request,
                           GURL* new_url) {
  if (!google_util::IsGoogleSearchUrl(request->url()) &&
      !google_util::IsGoogleHomePageUrl(request->url()))
    return;

  std::string query = request->url().query();
  std::string new_query = AddSafeSearchParameters(query);
  if (query == new_query)
    return;

  GURL::Replacements replacements;
  replacements.SetQueryStr(new_query);
  *new_url = request->url().ReplaceComponents(replacements);
}
