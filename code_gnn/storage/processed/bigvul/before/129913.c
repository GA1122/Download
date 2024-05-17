void DomDistillerViewerSource::RequestViewerHandle::DidFinishLoad(
    content::RenderFrameHost* render_frame_host,
    const GURL& validated_url) {
  if (render_frame_host->GetParent()) {
    return;
  }

  waiting_for_page_ready_ = false;
  if (!buffer_.empty()) {
    RunIsolatedJavaScript(web_contents()->GetMainFrame(), buffer_);
    buffer_.clear();
  }
}
