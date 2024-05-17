void BrowserRenderProcessHost::AddVisitedLinks(
    const VisitedLinkCommon::Fingerprints& links) {
  visited_link_updater_->AddLinks(links);
  if (visible_widgets_ == 0)
    return;

  visited_link_updater_->Update(this);
}
