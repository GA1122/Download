void DomDistillerViewerSource::RequestViewerHandle::DidNavigateMainFrame(
    const content::LoadCommittedDetails& details,
    const content::FrameNavigateParams& params) {
  const GURL& navigation = details.entry->GetURL();
  if (details.is_in_page || (navigation.SchemeIs(expected_scheme_.c_str()) &&
                             expected_request_path_ == navigation.query())) {
    return;
  }

  Cancel();
}
