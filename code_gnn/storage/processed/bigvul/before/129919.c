DomDistillerViewerSource::RequestViewerHandle::RequestViewerHandle(
    content::WebContents* web_contents,
    const std::string& expected_scheme,
    const std::string& expected_request_path,
    DistilledPagePrefs* distilled_page_prefs)
    : DomDistillerRequestViewBase(distilled_page_prefs),
      expected_scheme_(expected_scheme),
      expected_request_path_(expected_request_path),
      waiting_for_page_ready_(true) {
  content::WebContentsObserver::Observe(web_contents);
  distilled_page_prefs_->AddObserver(this);
}
