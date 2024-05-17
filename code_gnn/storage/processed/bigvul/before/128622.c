void ContextualSearchDelegate::BuildContext(
    const std::string& selection,
    bool use_resolved_search_term,
    content::WebContents* web_contents,
    bool may_send_base_page_url) {
  GURL page_url(web_contents->GetURL());
  GURL url_to_send;
  if (may_send_base_page_url &&
      CanSendPageURL(page_url, ProfileManager::GetActiveUserProfile(),
                     template_url_service_)) {
    url_to_send = page_url;
  }
  std::string encoding(web_contents->GetEncoding());
  context_.reset(new ContextualSearchContext(
      selection, use_resolved_search_term, url_to_send, encoding));
}
