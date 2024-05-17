void BrowserRenderProcessHost::ResetVisitedLinks() {
  visited_link_updater_->AddReset();
  if (visible_widgets_ == 0)
    return;

  visited_link_updater_->Update(this);
}
