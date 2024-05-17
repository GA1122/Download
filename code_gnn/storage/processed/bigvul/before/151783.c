Offliner::RequestStatus BackgroundLoaderOffliner::CanSavePageInBackground(
    content::WebContents* web_contents) {
  DCHECK(is_low_bar_met_)
      << "Minimum quality must have been reached before checking loaded page";
  std::unique_ptr<security_state::VisibleSecurityState> visible_security_state =
      GetVisibleSecurityState(web_contents);
  if (security_state::HasMajorCertificateError(*visible_security_state))
    return Offliner::RequestStatus::LOADED_PAGE_HAS_CERTIFICATE_ERROR;

  if (visible_security_state->malicious_content_status !=
      security_state::MaliciousContentStatus::MALICIOUS_CONTENT_STATUS_NONE) {
    return Offliner::RequestStatus::LOADED_PAGE_IS_BLOCKED;
  }

  if (GetPageType(web_contents) != content::PageType::PAGE_TYPE_NORMAL)
    return Offliner::RequestStatus::LOADED_PAGE_IS_CHROME_INTERNAL;

  return Offliner::RequestStatus::UNKNOWN;
}
