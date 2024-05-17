void DomDistillerViewerSource::RequestViewerHandle::SendJavaScript(
    const std::string& buffer) {
  if (waiting_for_page_ready_) {
    buffer_ += buffer;
  } else {
    DCHECK(buffer_.empty());
    if (web_contents()) {
      RunIsolatedJavaScript(web_contents()->GetMainFrame(), buffer);
    }
  }
}
