bool ExecuteCodeInTabFunction::CanExecuteScriptOnPage() {
  content::WebContents* contents = NULL;

  CHECK_GE(execute_tab_id_, 0);
  if (!GetTabById(execute_tab_id_, browser_context(), include_incognito(),
                  nullptr, nullptr, &contents, nullptr, &error_)) {
    return false;
  }

  CHECK(contents);

  int frame_id = details_->frame_id ? *details_->frame_id
                                    : ExtensionApiFrameIdMap::kTopFrameId;
  content::RenderFrameHost* rfh =
      ExtensionApiFrameIdMap::GetRenderFrameHostById(contents, frame_id);
  if (!rfh) {
    error_ = ErrorUtils::FormatErrorMessage(keys::kFrameNotFoundError,
                                            base::IntToString(frame_id),
                                            base::IntToString(execute_tab_id_));
    return false;
  }

  GURL effective_document_url(rfh->GetLastCommittedURL());
  bool is_about_url = effective_document_url.SchemeIs(url::kAboutScheme);
  if (is_about_url && details_->match_about_blank &&
      *details_->match_about_blank) {
    effective_document_url = GURL(rfh->GetLastCommittedOrigin().Serialize());
  }

  if (!effective_document_url.is_valid()) {
    return true;
  }

  if (!extension()->permissions_data()->CanAccessPage(
          extension(), effective_document_url, execute_tab_id_, &error_)) {
    if (is_about_url &&
        extension()->permissions_data()->active_permissions().HasAPIPermission(
            APIPermission::kTab)) {
      error_ = ErrorUtils::FormatErrorMessage(
          manifest_errors::kCannotAccessAboutUrl,
          rfh->GetLastCommittedURL().spec(),
          rfh->GetLastCommittedOrigin().Serialize());
    }
    return false;
  }

  return true;
}
