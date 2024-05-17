void RenderViewImpl::OnGetAllSavableResourceLinksForCurrentPage(
    const GURL& page_url) {
  std::vector<GURL> resources_list;
  std::vector<GURL> referrer_urls_list;
  std::vector<WebKit::WebReferrerPolicy> referrer_policies_list;
  std::vector<GURL> frames_list;
  webkit_glue::SavableResourcesResult result(&resources_list,
                                             &referrer_urls_list,
                                             &referrer_policies_list,
                                             &frames_list);

  if (!webkit_glue::GetAllSavableResourceLinksForCurrentPage(
          webview(),
          page_url,
          &result,
          const_cast<const char**>(content::GetSavableSchemes()))) {
    referrer_urls_list.clear();
    referrer_policies_list.clear();
    resources_list.clear();
    frames_list.clear();
  }

  std::vector<content::Referrer> referrers_list;
  CHECK_EQ(referrer_urls_list.size(), referrer_policies_list.size());
  for (unsigned i = 0; i < referrer_urls_list.size(); ++i) {
    referrers_list.push_back(
        content::Referrer(referrer_urls_list[i], referrer_policies_list[i]));
  }

  Send(new ViewHostMsg_SendCurrentPageAllSavableResourceLinks(routing_id(),
                                                              resources_list,
                                                              referrers_list,
                                                              frames_list));
}
