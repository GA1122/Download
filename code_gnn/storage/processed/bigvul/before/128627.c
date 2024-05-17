void ContextualSearchDelegate::GatherAndSaveSurroundingText(
    const std::string& selection,
    bool use_resolved_search_term,
    content::WebContents* web_contents,
    bool may_send_base_page_url) {
  GatherSurroundingTextWithCallback(
      selection, use_resolved_search_term, web_contents, may_send_base_page_url,
      base::Bind(&ContextualSearchDelegate::SaveSurroundingText, AsWeakPtr()));
}
